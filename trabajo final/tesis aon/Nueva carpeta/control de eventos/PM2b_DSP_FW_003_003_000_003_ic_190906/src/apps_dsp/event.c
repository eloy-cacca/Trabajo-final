// $Header
/// @file			event.c
/// @brief  		Event detection file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 February 2017
/// @details
/// @copyright
///

/// @addtogroup apps
/// @{

/**************************** INCLUDES ***************************************/

#include "event.h"
#include "../apps/system.h"
#include "../apps/meter_ctrl.h"
#include "../apps/unit_config.h"
#include "../apps/rtc.h"
#include "../apps/control.h"
#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

int64_t v_max_temp;
int64_t v_min_temp;
int64_t i_max_temp;

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Event routine running in each sample.
//
/// @param[inout] msp_ptr Pointer to incoming message
///
void event_buffer_init( event_buffer_t * event_buffer_ptr ) {

	memset( (void *)&event_buffer_ptr->state , 0 , sizeof(event_buffer_t) );
	event_buffer_ptr->state = EVENT_BUFFER_SAVE_B1;
	event_buffer_ptr->index_start = SETTINGS_EVENT_BUFFER_B1_START;
	event_buffer_ptr->index_stop  = SETTINGS_EVENT_BUFFER_B1_STOP;

}


/// @brief Event routine running in each sample.
//
/// @param[inout] event_ptr Pointer to input data structure.
/// @param[inout] event_det_ptr Pointer to input data structure.
/// @param[inout] data_ptr Pointer to input data structure.
/// @param[inout] data_processed_ptr Pointer to input data structure.
/// @param[inout] counter_ptr Pointer to control counter.
//
void event_update( event_buffer_t * event_buffer_ptr , event_det_t * event_det_ptr , int32_t * data_ptr , sp_t * data_processed_ptr , uint16_t * counter_ptr) {

//	static uint32_t snapshot_index = 0;
	volatile uint16_t type_temp = 0;

	///////////////////////////////////////////////////////////////////////////
	// Event Detection
	///////////////////////////////////////////////////////////////////////////

	// Always look current status to detect start and end of event
	if      ( ((data_processed_ptr->ch[CH_ADC_V1_N].rms2>>SETTINGS_DSP_NPLL_EXP) > unit_config.event_RMS2_over_voltage_threshold ) && (unit_config.event_flags & UNIT_CONFIG_EVENT_RMS_OV_V1_MASK ) ) {
		type_temp = EVENT_TYPE_RMS_OV_V1_MASK;
	}
	else if ( ((data_processed_ptr->ch[CH_ADC_V2_N].rms2>>SETTINGS_DSP_NPLL_EXP) > unit_config.event_RMS2_over_voltage_threshold ) && (unit_config.event_flags & UNIT_CONFIG_EVENT_RMS_OV_V2_MASK ) ) {
		type_temp = EVENT_TYPE_RMS_OV_V2_MASK;
	}
	else if ( ((data_processed_ptr->ch[CH_ADC_V3_N].rms2>>SETTINGS_DSP_NPLL_EXP) > unit_config.event_RMS2_over_voltage_threshold ) && (unit_config.event_flags & UNIT_CONFIG_EVENT_RMS_OV_V3_MASK ) ) {
		type_temp = EVENT_TYPE_RMS_OV_V3_MASK;
	}
	else if ( ((data_processed_ptr->ch[CH_ADC_V1_N].rms2>>SETTINGS_DSP_NPLL_EXP) < unit_config.event_RMS2_under_voltage_threshold ) && (unit_config.event_flags & UNIT_CONFIG_EVENT_RMS_UV_V1_MASK ) ) {
		type_temp = EVENT_TYPE_RMS_UV_V1_MASK;
	}
	else if ( ((data_processed_ptr->ch[CH_ADC_V2_N].rms2>>SETTINGS_DSP_NPLL_EXP) < unit_config.event_RMS2_under_voltage_threshold ) && (unit_config.event_flags & UNIT_CONFIG_EVENT_RMS_UV_V2_MASK ) ) {
		type_temp = EVENT_TYPE_RMS_UV_V2_MASK;
	}
	else if ( ((data_processed_ptr->ch[CH_ADC_V3_N].rms2>>SETTINGS_DSP_NPLL_EXP) < unit_config.event_RMS2_under_voltage_threshold ) && (unit_config.event_flags & UNIT_CONFIG_EVENT_RMS_UV_V3_MASK ) ) {
		type_temp = EVENT_TYPE_RMS_UV_V3_MASK;
	}
	else if ( ((data_processed_ptr->ch[CH_ADC_I1A].rms2>>SETTINGS_DSP_NPLL_EXP) > unit_config.event_RMS2_over_current_threshold ) && (unit_config.event_flags & UNIT_CONFIG_EVENT_RMS_OC_I1_MASK ) ) {
		type_temp = EVENT_TYPE_RMS_OC_I1_MASK;
	}
	else if ( ((data_processed_ptr->ch[CH_ADC_I2A].rms2>>SETTINGS_DSP_NPLL_EXP) > unit_config.event_RMS2_over_current_threshold ) && (unit_config.event_flags & UNIT_CONFIG_EVENT_RMS_OC_I2_MASK ) ) {
		type_temp = EVENT_TYPE_RMS_OC_I2_MASK;
	}
	else if ( ((data_processed_ptr->ch[CH_ADC_I3A].rms2>>SETTINGS_DSP_NPLL_EXP) > unit_config.event_RMS2_over_current_threshold ) && (unit_config.event_flags & UNIT_CONFIG_EVENT_RMS_OC_I3_MASK ) ) {
		type_temp = EVENT_TYPE_RMS_OC_I3_MASK;
	}

	// If meter control state is > METER_CTRL_STAE_LOW_VIN, type_temp is cleared. It avoid detection of a
	// new event and force a event stop is event buffer was waiting for it.
	if (( *meter_ctrl_get_state() > METER_CTRL_STATE_LOW_VIN) || (*meter_ctrl_get_state() == METER_CTRL_STATE_INIT))
		type_temp = 0;

	///////////////////////////////////////////////////////////////////////////
	// Event Detection - Finite State Machine (FSM)
	///////////////////////////////////////////////////////////////////////////

	switch(event_det_ptr->state){

		case EVENT_DET_STARTING: // Ignore any events until system is started

			// overwrite detection to avoid event buffer issues
			type_temp = 0;

			// Start after 4 period
			if ( *counter_ptr > (SETTINGS_DSP_NPLL<<2) ) {
				event_det_ptr->state = EVENT_DET_START;
			}
			break;

		case EVENT_DET_START: // Check start of event

//			snapshot_index = 0;
			if (type_temp != 0) {

				// reset snapshot index
//				snapshot_index = 0;

				// save type of event
				event_det_ptr->type = type_temp;

				// add buffer flag in type is buffer is in B1 state (buffer is storing new event)
				if (event_buffer_ptr->state == EVENT_BUFFER_SAVE_B1)
					event_det_ptr->type |= EVENT_TYPE_BUFFER_USED_MASK;


				// Save time
				copy_content( (uint8_t *)rtc_get_time(),
						      (uint8_t *)&event_det_ptr->time,
							  sizeof(rtc_t) );

		    	// update trend flag
				*ctrl_trend_flag_temp_ptr() |= SETTINGS_TREND_FLAGS_EVENT_START;

				// set init value for MAX and MIN
				v_max_temp = 0;
				v_min_temp = (int64_t)0x7FFFFFFFFFFFFFFF;
				i_max_temp = 0;

				// Next state
				event_det_ptr->state = EVENT_DET_END;
			}
			break;

		case EVENT_DET_END:	// Check end of event

			#if SETTINGS_ISR_TYPE
				event_det_ptr->duration_dsp_ticks += (CpuTimer0Regs.PRD.all + 1);
			#else
				event_det_ptr->duration_dsp_ticks += (epwm1_set_period + 1);
			#endif

			// save voltage max
			v_max_temp = MAX( v_max_temp , data_processed_ptr->ch[CH_ADC_V1_N].rms2 );
			v_max_temp = MAX( v_max_temp , data_processed_ptr->ch[CH_ADC_V2_N].rms2 );
			v_max_temp = MAX( v_max_temp , data_processed_ptr->ch[CH_ADC_V3_N].rms2 );

			// save voltage min
			v_min_temp = MIN( v_min_temp , data_processed_ptr->ch[CH_ADC_V1_N].rms2 );
			v_min_temp = MIN( v_min_temp , data_processed_ptr->ch[CH_ADC_V2_N].rms2 );
			v_min_temp = MIN( v_min_temp , data_processed_ptr->ch[CH_ADC_V3_N].rms2 );

			// save current max
			i_max_temp = MAX( i_max_temp , data_processed_ptr->ch[CH_ADC_I1A].rms2 );
			i_max_temp = MAX( i_max_temp , data_processed_ptr->ch[CH_ADC_I2A].rms2 );
			i_max_temp = MAX( i_max_temp , data_processed_ptr->ch[CH_ADC_I3A].rms2 );

			if (type_temp == 0) {

		    	// update trend flag
				*ctrl_trend_flag_temp_ptr() |= SETTINGS_TREND_FLAGS_EVENT_END;

				// Next state
				event_det_ptr->state = EVENT_DET_BACKGROUND_1;

			}
			break;

		case EVENT_DET_BACKGROUND_1:
			// do nothing, processing in background are going to set the following state (EVENT_DET_BACKGROUND_2).
			break;

		default: // EVENT_DET_BACKGROUND_2
			// do nothing, in background event structure are going to be cleared and then go to state EVENT_DET_START
			break;

	}

	///////////////////////////////////////////////////////////////////////////
	// Event Buffer
	///////////////////////////////////////////////////////////////////////////

	if ( (event_buffer_ptr->state != EVENT_BUFFER_WAITTING_READ) &&
	     (*counter_ptr & 0x1) ){								// Discard 1 sample

		// update index
		event_buffer_ptr->index++;
		if (event_buffer_ptr->index > event_buffer_ptr->index_stop )
			event_buffer_ptr->index = event_buffer_ptr->index_start;

		// update buffers
		event_buffer_ptr->buffer[event_buffer_ptr->index][TS_TICKS] = ((int16)*counter_ptr);//cputimer0_set_period);
		event_buffer_ptr->buffer[event_buffer_ptr->index][V1_AMP]   = ((int16)data_ptr[CH_ADC_V1_N]);
		event_buffer_ptr->buffer[event_buffer_ptr->index][V2_AMP]   = ((int16)data_ptr[CH_ADC_V2_N]);
		event_buffer_ptr->buffer[event_buffer_ptr->index][V3_AMP]   = ((int16)data_ptr[CH_ADC_V3_N]);
		event_buffer_ptr->buffer[event_buffer_ptr->index][VE_AMP]   = ((int16)data_ptr[CH_ADC_VE_N]);
		event_buffer_ptr->buffer[event_buffer_ptr->index][I1_AMP]   = ((int16)data_ptr[CH_ADC_I1A]);
		event_buffer_ptr->buffer[event_buffer_ptr->index][I2_AMP]   = ((int16)data_ptr[CH_ADC_I2A]);
		event_buffer_ptr->buffer[event_buffer_ptr->index][I3_AMP]   = ((int16)data_ptr[CH_ADC_I3A]);
		event_buffer_ptr->buffer[event_buffer_ptr->index][IN_AMP]   = ((int16)data_ptr[CH_ADC_IN]);

	}

	///////////////////////////////////////////////////////////////////////////
	// Event Buffer - Finite State Machine (FSM)
	///////////////////////////////////////////////////////////////////////////

	// Detect event and snapshot state machine
	switch(event_buffer_ptr->state){

		case EVENT_BUFFER_SAVE_B1:

			// if an event is detected, save index and go to next state
			if ( type_temp ) {

				// save where B1 was stopped
				event_buffer_ptr->det_B1_index = event_buffer_ptr->index;
//				event_buffer_ptr->det_B1_index |= 0x0003; // set bit0 and bit1 to ensure that index is in the end of a 4 data group (needed in event communication).

				// go to following state
				event_buffer_ptr->index       = SETTINGS_EVENT_BUFFER_B2_START - 1; // it is incremented first
				event_buffer_ptr->index_start = SETTINGS_EVENT_BUFFER_B2_START;
				event_buffer_ptr->index_stop  = SETTINGS_EVENT_BUFFER_B3_STOP;
				event_buffer_ptr->state       = EVENT_BUFFER_SAVE_B2B3;

			}
			break;

		case EVENT_BUFFER_SAVE_B2B3:

			// if it is the first time that a event cleared is detected
			if ( ( type_temp == 0) && (event_buffer_ptr->det_stop_index == 0) ) {

				// save where event stop was detected
				event_buffer_ptr->det_stop_index = event_buffer_ptr->index;
//				event_buffer_ptr->det_stop_index |= 0x0003; // set bit0 and bit1 to ensure that index is in the end of a 4 data group (needed in event communication).

				// B3 in order
				event_buffer_ptr->buffer_divided = 0;

			}

			// if final index of B3 is reach, check if event stop were detected and go to following state
			if ( event_buffer_ptr->index == (SETTINGS_EVENT_BUFFER_B3_STOP) )
				if ( event_buffer_ptr->det_stop_index ) {
					event_buffer_ptr->index       = SETTINGS_EVENT_BUFFER_B4_START - 1; // it is incremented first
					event_buffer_ptr->index_start = SETTINGS_EVENT_BUFFER_B4_START;
					event_buffer_ptr->index_stop  = SETTINGS_EVENT_BUFFER_B4_STOP;
					event_buffer_ptr->state       = EVENT_BUFFER_SAVE_B4;
				}
				else {
					event_buffer_ptr->index       = SETTINGS_EVENT_BUFFER_B3_START - 1; // it is incremented first
					event_buffer_ptr->index_start = SETTINGS_EVENT_BUFFER_B3_START;
					event_buffer_ptr->index_stop  = SETTINGS_EVENT_BUFFER_B3_STOP;
					event_buffer_ptr->state       = EVENT_BUFFER_SAVE_B3;
				}

			break;

		case EVENT_BUFFER_SAVE_B3:

			// if an event clearing is detected, save index and go to next state
			if ( type_temp == 0 ) {

				// save where stop event was detected
				event_buffer_ptr->det_stop_index = event_buffer_ptr->index;
//				event_buffer_ptr->det_stop_index |= 0x0003; // set bit0 and bit1 to ensure that index is in the end of a 4 data group (needed in event communication).

				// B3 in not in order
				event_buffer_ptr->buffer_divided = 1;

				// go to following state
				event_buffer_ptr->index       = SETTINGS_EVENT_BUFFER_B4_START  - 1; // it is incremented first;
				event_buffer_ptr->index_start = SETTINGS_EVENT_BUFFER_B4_START;
				event_buffer_ptr->index_stop  = SETTINGS_EVENT_BUFFER_B4_STOP;
				event_buffer_ptr->state       = EVENT_BUFFER_SAVE_B4;

			}
			break;

		case EVENT_BUFFER_SAVE_B4:

			// if B4 is full go to next state
			if ( event_buffer_ptr->index == (SETTINGS_EVENT_BUFFER_B4_STOP) ) {

				// set communication flag
				if (unit_config.mode_flags & UNIT_CONFIG_MODE_EVENT_MASK)
					system_flags.comm_dsp |= SETTINGS_COMM_RPI_DSP_REQUEST_EVENT_BUFFER;

				// go to following state
				event_buffer_ptr->state = EVENT_BUFFER_WAITTING_READ;

			}
			break;

		case EVENT_BUFFER_WAITTING_READ:
			// do nothing, in background event structure are going to be cleared and then start again
			break;

	}

}

/// @brief Event backgound routine
void event_backgound(event_buffer_t * event_buffer_ptr , event_det_t * event_det_ptr) {

	// Event detection
	if (event_det_ptr->state == EVENT_DET_BACKGROUND_1) {

		// post-processing
		event_det_ptr->v_max = sqrt( v_max_temp >> 7 );
		event_det_ptr->v_min = sqrt( v_min_temp >> 7 );
		event_det_ptr->i_max = sqrt( i_max_temp >> 7 );

		// set communication flag
		if (unit_config.mode_flags & UNIT_CONFIG_MODE_EVENT_MASK)
			system_flags.comm_dsp |= SETTINGS_COMM_RPI_DSP_REQUEST_EVENT_DET;

		// next state
		event_det_ptr->state = EVENT_DET_BACKGROUND_2;
	}
	else if (event_det_ptr->state == EVENT_DET_BACKGROUND_2) {

		// If communication flag was cleared, go to clear state and wait background process to continue
		if ( (system_flags.comm_dsp & SETTINGS_COMM_RPI_DSP_REQUEST_EVENT_DET ) == 0 ) {
			memset( (void *)event_det_ptr  , 0 , sizeof(event_det_t) );
			event_det_ptr->state = EVENT_DET_START;
		}
	}

	// Event buffers. If we are in the final state, check if flag in comm_flag was cleared to start again
	if (event_buffer_ptr->state == EVENT_BUFFER_WAITTING_READ)
		if ((system_flags.comm_dsp & SETTINGS_COMM_RPI_DSP_REQUEST_EVENT_BUFFER) == 0)
			event_buffer_init( event_buffer_ptr );

}

/// @}


