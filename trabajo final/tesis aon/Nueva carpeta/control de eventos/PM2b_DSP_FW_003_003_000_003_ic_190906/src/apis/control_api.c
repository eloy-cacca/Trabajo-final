// $Header
/// @file			control_api.c
/// @brief  		Control application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		02 November 2016
/// @details
/// @copyright
///

/// @addtogroup apis
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "control_api.h"

// local includes
#include "../apps/control.h"
#include "../apps/unit_config.h"
#include "../apps_dsp/signal_processing.h"
#include "../common_DSP_ARM/event_common.h"
#include "../utils/calculus.h"
#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

uint8_t send_trend( msg_t * msg_ptr , trend_t * trend_ptr ) {

	uint8_t payload_len;
	uint8_t * payload_ptr;
	uint16_t index = msg_ptr->packet_in[msg_h_payload_1st_byte] + (msg_ptr->packet_in[msg_h_payload_1st_byte+1]<<8);
	uint8_t response = 1; // used only in first command, last command and error command

    // Accept command only if payload length is equal to 2 bits
    if (msg_ptr->packet_in[msg_h_payload_len] == 2 )
    {

    	if (index == 0) {	// first command, it responses with 0x00
    		response = 0x00;
	    	payload_ptr = &response;
	    	payload_len = 1;
    	}
	    else if ( (index>0) && (index<(SETTINGS_TREND_LENGHT_IN_MSG-1)) ) { // trends are send
	    	payload_ptr = ( (uint16_t *)trend_ptr + ((index-1)<<6) ); // move pointer each 64 uint16_t (128 uint8_t)
    		payload_len = MIN( SETTINGS_TREND_LENGTH_IN_BYTE - ((index-1)<<7) , SETTINGS_MSG_LENGHT_PAYLOAD );
	    }
   		else if (index == (SETTINGS_TREND_LENGHT_IN_MSG-1)) {	// last command, it responses with 0xFF
    		response = 0xFF;
	    	payload_ptr = &response;
	    	payload_len = 1;
   		}
   		else {
   	    	// Send ERROR
   		    response = 0x01;
   		    payload_ptr = &response;
   		    payload_len = 1;
   		}


    } else {

    	// Send ERROR
	    response = 0x01;
	    payload_ptr = &response;
	    payload_len = 1;

	}

	comm_send_message( msg_ptr->sci_ptr ,
					   msg_ptr->packet_in[msg_h_src_id] ,
					   msg_ptr->packet_in[msg_h_msg_id]+1 ,
					   payload_len ,
					   payload_ptr
					   );

	// if this was the last msg, return with 1
	if (response == 0xFF)
		return 1;
	else
		return 0;

}

uint8_t send_event( msg_t * msg_ptr  ) {

	uint8_t payload_len;
	uint8_t * payload_ptr;
	event_buffer_t * event_buffer_ptr = ctrl_ptr_to_event_buffer();
	uint16_t index = msg_ptr->packet_in[msg_h_payload_1st_byte] + (msg_ptr->packet_in[msg_h_payload_1st_byte+1]<<8);
	uint16_t info[3];            	// used only in first command
	uint8_t response = 1; 			// used only in last command and error command. By default equal to 1 (error).

    // Accept command only if payload length is equal to 2 bits
    if (msg_ptr->packet_in[msg_h_payload_len] == 2 )
    {

    	// first command, it must responses with info
    	if (index == 0) {

    		info[0] = SETTINGS_EVENT_BUFFER_B1_STOP; 		// It is always B1 STOP
    		if (event_buffer_ptr->buffer_divided) {			// It depend on if buffer is divided or not
    			info[1] = SETTINGS_EVENT_BUFFER_B3_STOP;
    			info[2] = 1;
    		}
    		else {
    			info[1] = event_buffer_ptr->det_stop_index;
    			info[2] = 0;
    		}

    		payload_ptr = &info[0];
	    	payload_len = (sizeof(info)<<1);

    	}
    	// data from buffer, it must response with the request line
    	else if ( (index > 0) && ( index <= SETTINGS_EVENT_BUFFER_LENGHT ) ) {

    		int16_t i = index - 1;

    		// fix i depending on buffer
    		if ( (i >= SETTINGS_EVENT_BUFFER_B1_START) && ( i <= SETTINGS_EVENT_BUFFER_B1_STOP ) ) {

    			i += event_buffer_ptr->det_B1_index + 1;
    			if (i > SETTINGS_EVENT_BUFFER_B1_STOP)
    				i -= SETTINGS_EVENT_BUFFER_B2_START;

    		}
    		else if ( (i >= SETTINGS_EVENT_BUFFER_B3_START) && ( i <= SETTINGS_EVENT_BUFFER_B3_STOP ) && (event_buffer_ptr->buffer_divided == 1) ) {

    			i += event_buffer_ptr->det_stop_index - SETTINGS_EVENT_BUFFER_B3_START + 1;
    			if (i > SETTINGS_EVENT_BUFFER_B3_STOP)
    				i -= SETTINGS_EVENT_BUFFER_B2_START;

    		}

    		payload_ptr =  ( (uint16_t *)&( event_buffer_ptr->buffer[i][0] ) );
    		payload_len = (SETTINGS_EVENT_BUFFER_NUM<<1);

    	}
    	// last command, it must response with payload 0x00 and clear comunication flag.
    	else if (index == (SETTINGS_EVENT_BUFFER_LENGHT+1)) {

   		    response = 0;
   		    payload_ptr = &response;
   		    payload_len = 1;

    	}
   		else {
   	    	// Send ERROR
   		    response = 0x01;
   		    payload_ptr = &response;
   		    payload_len = 1;
   		}

    	comm_send_message( msg_ptr->sci_ptr ,
    					   msg_ptr->packet_in[msg_h_src_id] ,
    					   msg_ptr->packet_in[msg_h_msg_id]+1 ,
    					   payload_len ,
    					   payload_ptr
    					   );

    }

	// if this was the last msg, return with 1
	if (response == 0)
		return 1;
	else
		return 0;

}

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Request trend of 150 cycles
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_CTRL_trend_150cycles_req( msg_t * msg_ptr ) {

	if ( send_trend( msg_ptr , ctrl_ptr_to_trend_150cycles() ) )
		system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_REQUEST_TREND_3S; // clear comm flags

}

/// @brief Request trend of 5 minutes
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_CTRL_trend_5minutes_req( msg_t * msg_ptr ) {

	if ( send_trend( msg_ptr , ctrl_ptr_to_trend_5m() ) ) {
		system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_REQUEST_TREND_5M; // clear comm flags
#if SETTINGS_ARM_ENABLE_SDCARD_COMM_FLAG_SAVE
		system_flags.comm &= ~SETTINGS_COMM_FLAG_TEST01; // clear comm flags
		system_flags.comm &= ~SETTINGS_COMM_FLAG_TEST02; // clear comm flags
#endif
	}

}

/// @brief Request event buffer
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_CTRL_event_buffer_req( msg_t * msg_ptr ) {

	if ( send_event( msg_ptr ) )
		system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_REQUEST_EVENT_BUFFER; // clear comm flags

}


/// @brief Request event detection
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_CTRL_event_det_req( msg_t * msg_ptr ) {

	// Accept command only if:
	//     1) payload length is equal to 0
	//     3) ARM is waiting for this message
	if ( (msg_ptr->packet_in[msg_h_payload_len] == 0) &&
	     (system_flags.comm_dsp & SETTINGS_COMM_RPI_DSP_REQUEST_EVENT_DET) ) {

		// send msg
		comm_send_message( msg_ptr->sci_ptr ,
					   msg_ptr->packet_in[msg_h_src_id] ,
					   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	                   sizeof(event_det_t)<<1, //sizeof return the size in 16 bits words
	                   (uint8_t *)ctrl_ptr_to_event_det());

		// clear comm flags
		system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_REQUEST_EVENT_DET;
	}

}

/// @brief Request telemetria
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_CTRL_telemetria_req( msg_t * msg_ptr ) {

	typedef struct {
		int16_t RMS[8];
		int16_t dc_real[8];
		int16_t dc_imag[8];
	} telemetry_values_t;

	static telemetry_values_t values;
	trend_t * tred_ptr = ctrl_ptr_to_trend_150cycles();
	uint16_t index = 0;

	for ( index = 0 ; index<8 ; index++) {

		values.RMS[index]     = *tred_ptr->ch[index].rms;
		values.dc_real[index] = *tred_ptr->ch[index].harmonic[0].real;
		values.dc_imag[index] = *tred_ptr->ch[index].harmonic[0].imag;

	}

	comm_send_message( msg_ptr->sci_ptr ,
					   msg_ptr->packet_in[msg_h_src_id] ,
					   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	                   sizeof(telemetry_values_t)<<1, //sizeof return the size in 16 bits words
	                   (uint8_t *)&values
					   );

}




//****************************************************************************/

/// @}

