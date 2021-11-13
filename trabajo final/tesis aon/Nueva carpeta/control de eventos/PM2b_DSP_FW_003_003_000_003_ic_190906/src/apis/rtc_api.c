// $Header
/// @file			rtc_api.c
/// @brief  		Real Time Clock application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		24 Nobember 2016
/// @details
/// @copyright
///

/// @addtogroup apis
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "rtc_api.h"

// local includes
#include "../apps/unit_config.h"
#include "../apps/rtc.h"
#include "../apps/control.h"
#include "../utils/byte_string_operations.h"
#include "../apps_comm/app_layer.h"
#include "../apps_comm/comm.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief RTC date update response
///        This is a response message, so no response is needed.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_RTC_DSP_update_req( msg_t * msg_ptr ) {

	uint8_t response;
	rtc_t temp;

    // Accept command only if payload length is equal to rtc_t size in 8 bits
    if (msg_ptr->packet_in[msg_h_payload_len] == (sizeof(rtc_t)<<1) )
    {

    	// save rtc into a temporal register
    	copy_content_form_8bits_to_16bits( (uint16_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
			                               (uint16_t *)&temp ,
			                               msg_ptr->packet_in[msg_h_payload_len] );

    	// Set rtc with custom parameters
    	rtc_set_time(&temp);

    	// clear comm flags
    	system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_UPDATE_RTC;

    	// update trend flag
    	*ctrl_trend_flag_temp_ptr() |= SETTINGS_TREND_FLAGS_RTC_UPDATE;

   		// response
		response = 0x00;

    } else {

    	// response
	    response = 0x01;

	}

	comm_send_message( msg_ptr->sci_ptr ,
                       msg_ptr->packet_in[msg_h_src_id] ,
	                   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	                   1 ,
	                   &response
	                   );

}

/// @brief Get real time clock.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_RTC_DSP_send_req( msg_t * msg_ptr ) {

	comm_send_message( msg_ptr->sci_ptr ,
                       msg_ptr->packet_in[msg_h_src_id] ,
	                   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	                   sizeof(rtc_t)<<1, //sizeof return the size in 16 bits words and it must be the length in uint8_t
	                   (uint16_t *)rtc_get_time()
	                   );

}

#ifdef DELETE
/// @brief RTC date update request.
///
/// @param[in] msp_ptr Pointer to incoming message
///

void API_RTC_date_update_req(void) {

	comm_send_message( &SCIB ,
			           SETTINGS_ARM_ID ,
			           API_RTC_DATE_UPDATE_REQ ,
			           0 ,
					   0
					   );
}
#endif

/// @}

