// $Header
/// @file			dsp_api.c
/// @brief  		DSP application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		09 February 2017
/// @details
/// @copyright
///

/// @addtogroup apis
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "dsp_api.h"

// local includes
#include "../comm/comm.h"
#include "../comm/app_layer.h"
#include "../apps/unit_config.h"
#include "../apps/sdcard.h"
#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

///// @brief Request of DSP FW version.
void API_FW_DSP_version_req(void) {
	
	comm_send_msg( UART_DSP ,
				   SETTINGS_DSP_ID ,
				   API_FW_DSP_VERSION_REQ ,
				   0 ,
				   0);
		
	comm_TX_counter_set();
		
}

///// @brief Response to request of DSP FW version.
/////
///// @param[in] msp_ptr Pointer to incoming message
/////
void API_FW_DSP_version_res(msg_t * msg_ptr) {
	
	// Accept command only if:
	//     1) payload length is equal to 1 byte
	//     2) ARM is waiting for this message 
	if ((msg_ptr->packet_in[msg_h_payload_len] == 2) && ((comm_flags & SETTINGS_COMM_FLAG_FW_REQUEST_MASK) != 0 )) {
	
		// save DSP firmware version
		DSP_fw[0] = msg_ptr->packet_in[msg_h_payload_1st_byte];
		DSP_fw[1] = msg_ptr->packet_in[msg_h_payload_1st_byte+1];
		
		// Clear flag 
		comm_flags &= ~SETTINGS_COMM_FLAG_FW_REQUEST_MASK;
		
		// force next communication
		comm_TX_counter_clear();
		
	}
	
}

/****************************************************************************/

///// @brief Request of DSP comm flags.
void API_FW_DSP_send_comm_flag_req(void) {
	
	comm_send_msg( UART_DSP ,
	               SETTINGS_DSP_ID ,
			       API_FW_DSP_SEND_COMM_FLAG_REQ ,
		           0 ,
		           0);
				   
   comm_TX_counter_set();
	
}

///// @brief Response to request of DSP comm flags.
/////
///// @param[in] msp_ptr Pointer to incoming message
/////
void API_FW_DSP_send_comm_flag_res(msg_t * msg_ptr) {

	volatile uint16_t temp;

	gpio_set_pin_high(TP0_GPIO);

	// Accept command only if:
	//     1) payload length is equal to 2 byte
	//     2) ARM is waiting for this message
	if ((msg_ptr->packet_in[msg_h_payload_len] == 2) && (comm_flags & SETTINGS_COMM_FLAG_REQUEST_MASK)) {
		
		// save DSP comm flags
		temp = msg_ptr->packet_in[msg_h_payload_1st_byte] +  (msg_ptr->packet_in[msg_h_payload_1st_byte+1]<<8);
		comm_flags |= temp;
		
		// Update flag to avoid
		comm_flags &= ~SETTINGS_COMM_FLAG_REQUEST_MASK;
		
#if SETTINGS_ARM_ENABLE_SDCARD_COMM_FLAG_SAVE
		// test
		sdcard_save_flags(temp);
#endif
		
		// force next communication
		comm_TX_counter_clear();
		
	}
	
	gpio_set_pin_low(TP0_GPIO);
	
}

/****************************************************************************/

//// @brief Request of update DSP RTC.
void API_RTC_DSP_update_req(void) {
	
	// Real Time clock variable Definition
	typedef struct {

		uint16_t m_second;	  // 14bits (0..999)
		uint16_t second;	  // 6bits  (0..59)
		uint16_t minute;	  // 6bits  (0..59)
		uint16_t hour;		  // 5bits  (0..23)

	} dsp_rtc_t;	
	static dsp_rtc_t dsp_rtc;
	
	// temporal data
	uint32_t temp_h,temp_m,temp_s;

	// Get time	
	rtc_get_time( RTC, &temp_h , &temp_m , &temp_s );
	
	// update data to by send
	dsp_rtc.m_second = 0;
	dsp_rtc.second = temp_s;
	dsp_rtc.minute = temp_m;
	dsp_rtc.hour = temp_h;
	
	comm_send_msg( UART_DSP ,
				   SETTINGS_DSP_ID ,
				   API_RTC_DSP_UPDATE_REQ ,
				   sizeof(dsp_rtc_t) ,
				   (uint8_t *)&dsp_rtc
				 );
				 
	comm_TX_counter_set();			 
	
}

///// @brief Response to request of update DSP RTC.
/////
///// @param[in] msp_ptr Pointer to incoming message
/////
void API_RTC_DSP_update_res(msg_t * msg_ptr) {
	
	// Accept command only if:
	//     1) payload length is equal to 1 byte
	//     2) payload is equal to 0 (NO ERROR)
	//     3) ARM is waiting for this message
	if ( (msg_ptr->packet_in[msg_h_payload_len] == 1) &&
	     (msg_ptr->packet_in[msg_h_payload_1st_byte] == 0) &&
	     (comm_flags & SETTINGS_COMM_FLAG_RTC_REQUEST_MASK) ) {
					
		// Update flag to avoid
		comm_flags &= ~SETTINGS_COMM_FLAG_RTC_REQUEST_MASK;
		
		// force next communication
		comm_TX_counter_clear();
			
	}
	
}

/****************************************************************************/

//// @brief Request of update DSP unit config.
void API_UNIT_CONFIG_DSP_update_req(void) {
	
	comm_send_msg(	UART_DSP ,
					SETTINGS_DSP_ID ,
					API_UNIT_CONFIG_DSP_UPDATE_REQ ,
					sizeof(unit_config_t),
					(uint8_t *)&unit_config);
	
	comm_TX_counter_set();
}

///// @brief Response to request of update DSP unit config.
/////
///// @param[in] msp_ptr Pointer to incoming message
/////
void API_UNIT_CONFIG_DSP_update_res(msg_t * msg_ptr) {

	// Accept command only if:
	//     1) payload length is equal to 1 byte
	//     2) payload is equal to 0 (NO ERROR)
	//     3) ARM is waiting for this message
	if ( (msg_ptr->packet_in[msg_h_payload_len] == 1) &&
	     (msg_ptr->packet_in[msg_h_payload_1st_byte] == 0) &&
	     (comm_flags & SETTINGS_COMM_FLAG_UNIT_CONFIG_REQUEST_MASK) ) {
					
		// Update flag to avoid
		comm_flags &= ~SETTINGS_COMM_FLAG_UNIT_CONFIG_REQUEST_MASK;
		
		// force next communication
		comm_TX_counter_clear();
			
	}
	
}

/****************************************************************************/

//// @brief Request of update DSP unit calibration.
void API_UNIT_CALIBRATION_DSP_update_req(void) {
	
	comm_send_msg(	UART_DSP ,
	                SETTINGS_DSP_ID ,
	                API_UNIT_CALIBRATION_DSP_UPDATE_REQ ,
                    sizeof(unit_calibration_t),
	                (uint8_t *)&unit_calibration);

	comm_TX_counter_set();
	
}

///// @brief Response to request of update DSP unit calibration.
/////
///// @param[in] msp_ptr Pointer to incoming message
/////
void API_UNIT_CALIBRATION_DSP_update_res(msg_t * msg_ptr) {

	// Accept command only if:
	//     1) payload length is equal to 1 byte
	//     2) payload is equal to 0 (NO ERROR)
	//     3) ARM is waiting for this message
	if ( (msg_ptr->packet_in[msg_h_payload_len] == 1) &&
	     (msg_ptr->packet_in[msg_h_payload_1st_byte] == 0) &&
	     (comm_flags & SETTINGS_COMM_FLAG_UNIT_CALIBRATION_REQUEST_MASK) ) {
		
		// Update flag to avoid
		comm_flags &= ~SETTINGS_COMM_FLAG_UNIT_CALIBRATION_REQUEST_MASK;
		
		// force next communication
		comm_TX_counter_clear();
		
	}
	
}

/****************************************************************************/

uint16_t trend_150cycles_request_counter = 0;

//// @brief Request of 150 cycles trend.
void API_CTRL_DSP_trend_150cycles_req(void) {
	
	// send command only if there is not pending a previous sd-card task
	if (0 == ( sdcard_flags_task & SDCARD_FLAGS_TASK_SAVE_TRENDS_150CYCLES )) {
	
		comm_send_msg(  UART_DSP ,
						SETTINGS_DSP_ID ,
						API_CTRL_DSP_TREND_150CYCLES_REQ ,
						sizeof(trend_150cycles_request_counter) ,
						(uint8_t *)&trend_150cycles_request_counter);
	
		comm_TX_counter_set();
	
	}
	
}

///// @brief Response to request of 150 cycles trend.
/////
///// @param[in] msp_ptr Pointer to incoming message
/////
void API_CTRL_DSP_trend_150cycles_res(msg_t * msg_ptr) {	
	
	// Accept command only if ARM is waiting for this message
	if (comm_flags & SETTINGS_COMM_FLAG_TREND_150CYCLE_MASK) {

		// First msg (trend_request_counter=0). It must return 0x00.
		if ( (trend_150cycles_request_counter==0) && (msg_ptr->packet_in[msg_h_payload_len] == 1) && (msg_ptr->packet_in[msg_h_payload_1st_byte] == 0x00)) {
			 trend_150cycles_request_counter++;
		}
		// Trend data (0 < trend_request_counter < (SETTINGS_TREND_MSG_NUMBER-1) ). Save data into trend structure.
		else if ((trend_150cycles_request_counter>0) && (trend_150cycles_request_counter<(SETTINGS_TREND_LENGHT_IN_MSG-1))) {
			copy_content( &msg_ptr->packet_in[msg_h_payload_1st_byte] , (((uint8_t *)&sdcard_trend_150cycles)+((trend_150cycles_request_counter-1)<<7)) , msg_ptr->packet_in[msg_h_payload_len] );
			trend_150cycles_request_counter++;
		}
		// Last msg. It must return 0xFF.
		else if ((trend_150cycles_request_counter==(SETTINGS_TREND_LENGHT_IN_MSG-1)) && (msg_ptr->packet_in[msg_h_payload_len] == 1) && (msg_ptr->packet_in[msg_h_payload_1st_byte] = 0xFF)) {
			trend_150cycles_request_counter++;
		}
		// If no one, do nothing.

		// If trend structure is complete, clear communication flag and set sd-card task flag to force 
		// data storing in background.
		if (trend_150cycles_request_counter == SETTINGS_TREND_LENGHT_IN_MSG) {
				
			// Update flags
			comm_flags &= ~SETTINGS_COMM_FLAG_TREND_150CYCLE_MASK;
			sdcard_flags_task |= SDCARD_FLAGS_TASK_SAVE_TRENDS_150CYCLES;	
			
			// Reset counter
			trend_150cycles_request_counter = 0;

		}	
	
	}

	// force next communication
	comm_TX_counter_clear();
	
}

/****************************************************************************/

uint16_t trend_5minutes_request_counter = 0;

//// @brief Request of 5 minutes trend.
void API_CTRL_DSP_trend_5minutes_req(void) {

	// send command only if there is not pending a previous sd-card task
	if (0 == ( sdcard_flags_task & SDCARD_FLAGS_TASK_SAVE_TRENDS_5MINUTES )) {

		comm_send_msg(  UART_DSP ,
				        SETTINGS_DSP_ID ,
						API_CTRL_DSP_TREND_5MINUTES_REQ ,
						sizeof(trend_5minutes_request_counter) ,
						(uint8_t *)&trend_5minutes_request_counter);
	
		comm_TX_counter_set();

	}
	
}

///// @brief Response to request of 5 minutes trend.
/////
///// @param[in] msp_ptr Pointer to incoming message
/////
void API_CTRL_DSP_trend_5minutes_res(msg_t * msg_ptr) {
	
	// Accept command only if ARM is waiting for this message
	if (comm_flags & SETTINGS_COMM_FLAG_TREND_5MINUTES_MASK) {

		// First msg (trend_request_counter=0). It must return 0x00.
		if ( (trend_5minutes_request_counter==0) && (msg_ptr->packet_in[msg_h_payload_len] == 1) && (msg_ptr->packet_in[msg_h_payload_1st_byte] == 0x00)) {
			trend_5minutes_request_counter++;
		}
		// Trend data (0 < trend_request_counter < (SETTINGS_TREND_MSG_NUMBER-1) ). Save data into trend structure.
		else if ((trend_5minutes_request_counter>0) && (trend_5minutes_request_counter<(SETTINGS_TREND_LENGHT_IN_MSG-1))) {
			copy_content( &msg_ptr->packet_in[msg_h_payload_1st_byte] , (((uint8_t *)&sdcard_trend_5minutes)+((trend_5minutes_request_counter-1)<<7)) , msg_ptr->packet_in[msg_h_payload_len] );
			trend_5minutes_request_counter++;
		}
		// Last msg. It must return 0xFF.
		else if ((trend_5minutes_request_counter==(SETTINGS_TREND_LENGHT_IN_MSG-1)) && (msg_ptr->packet_in[msg_h_payload_len] == 1) && (msg_ptr->packet_in[msg_h_payload_1st_byte] = 0xFF)) {
			trend_5minutes_request_counter++;
		}
		// If no one, do nothing.

		// If trend structure is complete, clear communication flag and set sd-card task flag to force
		// data storing in background.
		if (trend_5minutes_request_counter == SETTINGS_TREND_LENGHT_IN_MSG) {
			
			// Update flags
			comm_flags &= ~SETTINGS_COMM_FLAG_TREND_5MINUTES_MASK;
			sdcard_flags_task |= SDCARD_FLAGS_TASK_SAVE_TRENDS_5MINUTES;
			
			// Reset counter
			trend_5minutes_request_counter = 0;

		}
		
	}

	// force next communication
	comm_TX_counter_clear();
	
}

/****************************************************************************/

uint16_t event_request_counter = 0;

//// @brief Request of event buffer.
void API_CTRL_DSP_event_buffer_req(void) {
	
	// send command only if there is not pending a previous sd-card task
	if (0 == ( sdcard_flags_task & (SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER_INFO | SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER) )) {
		
		comm_send_msg(  UART_DSP ,
						SETTINGS_DSP_ID ,
						API_CTRL_DSP_EVENT_BUFFER_REQ ,
						sizeof(event_request_counter) ,
						(uint8_t *)&event_request_counter);
						
		comm_TX_counter_set();
	}

}

///// @brief Response to request of event buffer.
/////
///// @param[in] msp_ptr Pointer to incoming message
/////
void API_CTRL_DSP_event_buffer_res(msg_t * msg_ptr) {
	
	// Accept command only if ARM is waiting for this message
	if (comm_flags & SETTINGS_COMM_FLAG_EVENT_BUFFER_MASK) {

		// First msg (event_request_counter=0).
		if ( (event_request_counter==0) && (msg_ptr->packet_in[msg_h_payload_len] == sizeof(sdcard_event_buffer_info) ) ) {
			copy_content( &msg_ptr->packet_in[msg_h_payload_1st_byte] , ((uint8_t *)&sdcard_event_buffer_info[0]) , sizeof(sdcard_event_buffer_info) );
			sdcard_flags_task |= SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER_INFO;
			event_request_counter++;
		}
		// event data (0 < event_request_counter < (SETTINGS_EVENT_LENGHT_IN_MSG-1) ). Save data into trend structure.
		else if ( (event_request_counter>0) && (event_request_counter<=SETTINGS_EVENT_BUFFER_LENGHT) && (msg_ptr->packet_in[msg_h_payload_len] == sizeof(sdcard_event_buffer)) ) {
			copy_content( &msg_ptr->packet_in[msg_h_payload_1st_byte] , ((uint8_t *)&sdcard_event_buffer[0]) , sizeof(sdcard_event_buffer) );
			sdcard_flags_task |= SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER;
			event_request_counter++;
		}
		// Last msg. It must return 0.
		else if ((event_request_counter==(SETTINGS_EVENT_BUFFER_LENGHT+1)) && (msg_ptr->packet_in[msg_h_payload_len] == 1) && (msg_ptr->packet_in[msg_h_payload_1st_byte] == 0)) {
			event_request_counter=0;		
			comm_flags &= ~SETTINGS_COMM_FLAG_EVENT_BUFFER_MASK;
		}
		
	}

	// force next communication
	comm_TX_counter_clear();

}

/****************************************************************************/

//// @brief Request of event detection info.
void API_CTRL_DSP_event_det_req(void) {
	
	// send command only if there is not pending a previous sd-card task
	if (0 == ( sdcard_flags_task & SDCARD_FLAGS_TASK_SAVE_EVENT_DET_INFO )) {
	
		comm_send_msg(  UART_DSP ,
						SETTINGS_DSP_ID ,
						API_CTRL_DSP_EVENT_DET_REQ ,
						0 ,
						0);
					
		comm_TX_counter_set();

	}
	
}

///// @brief Response to request of detection info.
/////
///// @param[in] msp_ptr Pointer to incoming message
/////
void API_CTRL_DSP_event_det_res(msg_t * msg_ptr) {
	
	// Accept command only if:
	//     1) payload length is equal to sizeof(event_det_t)
	//     2) ARM is waiting for this message
	if ( (msg_ptr->packet_in[msg_h_payload_len] == sizeof(event_det_t)) &&
	     (comm_flags & SETTINGS_COMM_FLAG_EVENT_DET_MASK) ) {

		// copy data to temporal variable
		copy_content( &msg_ptr->packet_in[msg_h_payload_1st_byte] ,
		              (uint8_t *)&sdcard_event_det ,
					  sizeof(event_det_t));

		// Update flags
		comm_flags &= ~SETTINGS_COMM_FLAG_EVENT_DET_MASK;
		sdcard_flags_task |= SDCARD_FLAGS_TASK_SAVE_EVENT_DET_INFO;
		
		// force next communication
		comm_TX_counter_clear();

	}
	
}

/****************************************************************************/

/// @}

