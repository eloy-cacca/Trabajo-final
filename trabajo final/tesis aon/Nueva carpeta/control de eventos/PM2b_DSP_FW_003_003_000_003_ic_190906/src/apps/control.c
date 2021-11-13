// $Header
/// @file			control.c
/// @brief  		Control and processing module.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		29 February 2016
/// @details
/// @copyright
///

/// @addtogroup modules
/// @{

/**************************** INCLUDES ***************************************/

#include "control.h"
#include "system.h"
#include "unit_config.h"

#include "../apps/rtc.h"
#include "../apps/vspt.h"
#include "../apps/meter_ctrl.h"
#include "../apps_dsp/signal_adc.h"
#include "../apps_dsp/signal_processing.h"
#include "../apps_dsp/index.h"
#include "../apps_dsp/trend.h"
#include "../apps_dsp/event.h"

#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

uint16_t control_counter;					// service routine counter

int32_t signals[CH_ADC_TOTAL_NUM];			// sensors register

sp_t processed_signals; 	        		// processed signals

index_accum_t  index_10cycle_accum[2];		// index accumulator
index_result_t index_10cycle_result;		// index result

trend_accum_t  trend_150cycle_accum;		// trend accumulator
trend_t trend_150cycle_result;    	        // trend result

trend_accum_t  trend_5m_accum;				// trend accumulator
trend_t trend_5m_result;				    // trend result

uint16_t trend_flag_temp;					// trend flag - temporal register

event_buffer_t event_buffer;				// event structure
event_det_t event_det;						// event detection structure

vspt_t vspt_s;								// sampling frequency structure

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/


/// @brief Control module initialization.
/// @details Initialization of local variables of control module.
void control_init(void) {

	// init sampling period with set point
#if SETTINGS_ISR_TYPE
	cputimer0_set_period = unit_config.Ts_ticks_Q5_SP>>5;
#else
	epwm1_set_period = unit_config.Ts_ticks_Q5_SP>>5;
#endif

	// clear variable
	control_counter = 0;
	memset( (void *)&signals               , 0 , sizeof(signals) );
	memset( (void *)&processed_signals     , 0 , sizeof(processed_signals) );

	// index
	memset( (void *)&index_10cycle_accum   , 0 , sizeof(index_10cycle_accum) );
	memset( (void *)&index_10cycle_result  , 0 , sizeof(index_10cycle_result) );

	// trend 150 cycles
	trend_reset_accum( &trend_150cycle_accum );
	trend_reset_result( &trend_150cycle_result );

	// trend 5m
	trend_reset_accum( &trend_5m_accum );
	trend_reset_result( &trend_5m_result );

	// trend flag temporal register
	trend_flag_temp = 0;

	// Event buffer
	event_buffer_init( &event_buffer );

	// Event detection
	memset( (void *)&event_det  , 0 , sizeof(event_det_t) );
	event_det.state = EVENT_DET_STARTING;

	// Sampling frequency
	memset( (void *)&vspt_s  , 0 , sizeof(vspt_s) );

}


/// @brief Control module start.
void control_start(void) {

	// Enable interrupt
	DeviceEnableInterrupts();

	// start control
#if SETTINGS_ISR_TYPE
	StartCpuTimer0();         // start cputimer0
#else
	epwm1_start0();           // start pwm
#endif

	// update trend flag
	*ctrl_trend_flag_temp_ptr() |= SETTINGS_TREND_FLAGS_INIT;

}


/// @brief Control module routine.
/// @details Several control and processing routine.
extern void control_update(void) {

	// trend accumulator id
	static uint16_t id = 0;

	// Time utilization measurement start
	StartCpuTimer1();

	// sensor update
	signal_adc_update(signals);

	// update control counter
	control_counter++;

	// Update led
	if (control_counter & 0x2000)
		dsp_red_led_control_tog();
	if (system_flags.comm_dsp) {
		dsp_red_led_comm_on();
		SET_FLAG( system_flags.state_dsp , SETTINGS_STATE_DSP_PENDING_DATA );
	}
	else {
		dsp_red_led_comm_off();
		CLEAR_FLAG( system_flags.state_dsp , SETTINGS_STATE_DSP_PENDING_DATA );
	}

	// Update Real Time Clock
	rtc_update();

	// meter control
//	meter_ctrl_update(&signals[CH_ADC_VIN_Q11]);

	// pre-processing voltage
	processing_update( signals , &processed_signals );

	// update sampling frequency
	vspt( &vspt_s , &processed_signals );

	// Update the 10 cycles index accumulators.
	// NOTE: Two accumulator are used since the processing of data in background could
	// not be finished between consecutive sampling instants. if one accumulator is
	// complete, set the other one for update the index. One accumulator is always updates.
	{

		// Select accumulator
		if ( index_10cycle_accum[0].accum == SETTINGS_INDEX_10CYCLE_ACCUM_MAX_DATA )
			id = 1;
		else if ( index_10cycle_accum[1].accum == SETTINGS_INDEX_10CYCLE_ACCUM_MAX_DATA )
			id = 0;

		// continue with the accumulation of data. If accumulator is not empty do nothings (this is a error since accumulator should be empty)
		if ( index_10cycle_accum[id].accum < SETTINGS_INDEX_10CYCLE_ACCUM_MAX_DATA )
			index_update_accum( &processed_signals , &index_10cycle_accum[id]  );
	}

	// Event routine. If an event is detected a save state is computed
	event_update( &event_buffer , &event_det , &signals[0] , &processed_signals , &control_counter );

	// Debugging
	dac1_16bits_signed( (int16_t)signals[0] );
	dac2_16bits_signed( (int16_t)((epwm1_set_period - 23437)<<5) );

	// Time utilization measurement stop
	StopCpuTimer1();
	index_10cycle_accum[id].time_utilization = MAX( index_10cycle_accum[id].time_utilization , 0xFFFFFFFF - ReadCpuTimer1Counter() );
	ReloadCpuTimer1();

}

/// @brief Background control module routine.
/// @details Several control and processing routine running in background.
void 	control_background(void) {

	// 10 cycles index update (1280 sampling instant)
	// Update index result if one accumulator is complete. If a result of 10 cycles
	// index is updated, the 5 minutes index accumulator is updated too.
	if (index_10cycle_accum[0].accum == SETTINGS_INDEX_10CYCLE_ACCUM_MAX_DATA) {
		index_update_result( &index_10cycle_accum[0] , &index_10cycle_result );
		trend_update_accum_from_index( &index_10cycle_result , &trend_150cycle_accum );
	}
	else if (index_10cycle_accum[1].accum == SETTINGS_INDEX_10CYCLE_ACCUM_MAX_DATA) {
		index_update_result( &index_10cycle_accum[1] , &index_10cycle_result );
		trend_update_accum_from_index( &index_10cycle_result , &trend_150cycle_accum );
	}

	// 150 cycles trend update (15 samples of 10 cycle index)
	// Update trend result if accumulator is complete. If a result of 150 cycles
	// trend is updated, the 5 minutes trend accumulator is updated too.
	if (trend_150cycle_accum.accum == SETTINGS_TREND_150CYCLE_ACCUM_MAX_DATA) {
		GpioDataRegs.GPASET.bit.GPIO21 = 1;//test_point_03_set();
		trend_update_result( &trend_150cycle_accum , &trend_150cycle_result );
		trend_update_accum( &trend_150cycle_result , &trend_5m_accum );
		if (unit_config.mode_flags & UNIT_CONFIG_MODE_TREND_150CYCLES_MASK)
			system_flags.comm_dsp |= SETTINGS_COMM_RPI_DSP_REQUEST_TREND_3S;
		GpioDataRegs.GPACLEAR.bit.GPIO21 = 1;//test_point_03_clr();
	}

	// 5 minutes index update. If RTC set a 5 minutes flag, 5 minute result is
	// computed.
	if ( rtc_alarm5minute_flag() == 1 ) {
		trend_update_result( &trend_5m_accum , &trend_5m_result );
#if SETTINGS_ARM_ENABLE_SDCARD_COMM_FLAG_SAVE
		comm_flags |= SETTINGS_COMM_FLAG_TEST02;
#endif
		if (unit_config.mode_flags & UNIT_CONFIG_MODE_TREND_5MINUTES_MASK)
			system_flags.comm_dsp |= SETTINGS_COMM_RPI_DSP_REQUEST_TREND_5M;
	}

	// event. If it was sent to ARM, clear structure
	event_backgound( &event_buffer , &event_det );

}


/// @brief Send trend pointer.
trend_t * ctrl_ptr_to_trend_150cycles(void) {

	return &trend_150cycle_result;

}

/// @brief Send trend pointer.
trend_t * ctrl_ptr_to_trend_5m(void) {

	return &trend_5m_result;

}

/// @brief Send event pointer.
event_buffer_t * ctrl_ptr_to_event_buffer(void) {

	return &event_buffer;

}

/// @brief Send event detection pointer.
event_det_t * ctrl_ptr_to_event_det(void) {

	return &event_det;

}

/// @brief Update index result flags
uint16_t * ctrl_trend_flag_temp_ptr(void) {

	return &trend_flag_temp;

}

/// @}


















  




















