// $Header
/// @file			rtc_api.c
/// @brief  		Real Time Clock application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		29 December 2016
/// @details
/// @copyright
///

/// @addtogroup apis
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "rtc_api.h"

// local includes
#include "../comm/app_layer.h"
#include "../apps/unit_config.h"
#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

typedef struct {

	uint32_t year;
	uint32_t month;
	uint32_t day;
	uint32_t week;
	
	uint32_t hour;
	uint32_t minute;
	uint32_t second;

} rtc_time_and_date_t;

/**************************** LOCAL VARIABLES ********************************/

rtc_time_and_date_t data;

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Send real time clock.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_RTC_ARM_send_req( msg_t * msg_ptr ) {
	
	// get time and date
	rtc_get_time(RTC, &data.hour, &data.minute, &data.second);
	rtc_get_date(RTC, &data.year, &data.month, &data.day, &data.week);

	comm_send_msg( msg_ptr->uart_ptr ,
                   msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               sizeof(rtc_time_and_date_t),
	               (uint8_t *)&data);

/*	comm_send_msg( msg_ptr->uart_ptr ,
                   msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               sizeof(rtc_time_and_date_t),
	               (uint8_t *)&temp_data);*/
}

void API_RTC_ARM_update_req( msg_t * msg_ptr ) {

	uint8_t response;

    // Accept command only if payload length is equal to 0
    if (msg_ptr->packet_in[msg_h_payload_len] == sizeof(rtc_time_and_date_t) )
    {

    	// save rtc into a temporal register
    	copy_content( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
			          (uint8_t *)&data ,
				      sizeof(rtc_time_and_date_t) );

    	// Set rtc with custom parameters
   		rtc_set_time(RTC,data.hour,data.minute,data.second);
   		rtc_set_date(RTC,data.year,data.month,data.day,data.week);

		// set flag to force DSP update in background
		comm_flags |= SETTINGS_COMM_FLAG_RTC_REQUEST_MASK;

		// response
		response = 0x00;

	}
	else
	{

		// response
		response = 0x01;

	}

	comm_send_msg( msg_ptr->uart_ptr ,
	               msg_ptr->packet_in[msg_h_src_id] ,
                   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               1 ,
	               &response);

}

/// @}

