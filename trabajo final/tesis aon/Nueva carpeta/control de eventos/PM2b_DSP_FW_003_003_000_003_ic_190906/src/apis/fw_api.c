// $Header
/// @file			fw_api.c
/// @brief  		Firmware application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		05 August 2016
/// @details
/// @copyright
///

/// @addtogroup apis
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "fw_api.h"

// local includes
#include "../apps/unit_config.h"
#include "../apps_comm/app_layer.h"
#include "../apps_comm/comm.h"
#include "../apps/rtc.h"
#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Response to FW version request.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_FW_version_req( msg_t * msg_ptr )
{

	static uint16_t fw_version = SETTINGS_DSP_FW_VERSION | (SETTINGS_DSP_FW_VERSION_ENG_BUILD<<8);

	// clear comm flags
	system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_REQUEST_FW;

	comm_send_message( msg_ptr->sci_ptr ,
                       msg_ptr->packet_in[msg_h_src_id] ,
			           msg_ptr->packet_in[msg_h_msg_id]+1 ,
			           2 , // length in uint8
					   (uint16_t *)&fw_version);

}

/// @brief Send echo back to source (64 bytes max).
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_FW_echo_req( msg_t * msg_ptr )
{
	int16_t temp_16bits[64];

	copy_content_form_8bits_to_16bits( (uint16_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
		                               (uint16_t *)&temp_16bits ,
			                           128 ); // 64 uint8_t -> 32 uint16_t

	comm_send_message( msg_ptr->sci_ptr ,
                       msg_ptr->packet_in[msg_h_src_id] ,
	                   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	                   msg_ptr->packet_in[msg_h_payload_len] ,
	                   (uint16_t *)&temp_16bits
	                   );

}

/// @brief Send comm flags.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_FW_dsp_send_comm_flag_req( msg_t * msg_ptr ) {

	// Accept command only if payload length is equal to 2
	if (msg_ptr->packet_in[msg_h_payload_len] == 3) {

		// save comm flags and dsp state
		uint16_t responde[2];
		responde[0] = system_flags.comm_dsp;
		responde[1] = system_flags.state_dsp;
		// save arpi and rpi state
		system_flags.state_rpi = msg_ptr->packet_in[msg_h_payload_1st_byte];
		system_flags.state_arm = msg_ptr->packet_in[msg_h_payload_1st_byte+1];
		//system_flags.state_dsp = msg_ptr->packet_in[msg_h_payload_1st_byte+2];  NO UPDATE
		// send response
		comm_send_message( msg_ptr->sci_ptr ,
	                       msg_ptr->packet_in[msg_h_src_id] ,
		                   msg_ptr->packet_in[msg_h_msg_id]+1 ,
		                   3,
		                   (uint16_t *)&responde
		                   );

	}

}

/// @brief Msg ID unknown. Received ID is send in the payload.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_FW_unknow_msg_id( msg_t * msg_ptr ) {

	// Do not send a error message (id unknown) if DSP is the source.
	if (msg_ptr->packet_in[msg_h_src_id] != SETTINGS_ARM_ID ) {

		comm_send_message( msg_ptr->sci_ptr ,
						   msg_ptr->packet_in[msg_h_src_id] ,
						   API_FW_DSP_UNKNOW_MSG_ID_RES ,
						   1 ,
						   (uint16_t *)&msg_ptr->packet_in[msg_h_msg_id]
						   );

	}

}

#ifdef DELETE
/// @brief Send unit start command to ARM.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_FW_unit_start_send( void )
{

	static uint16_t command[1] = {SETTINGS_FW_VERSION};

	comm_send_message( &SCIB ,
			           SETTINGS_ARM_ID ,
			           API_FW_UNIT_START_SEND ,
			           1 ,
					   (uint16_t *)&command
					   );

}

/// @brief Receive unit start ack.
///        This is a response message, so no response is needed.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_FW_unit_start_ack( msg_t * msg_ptr )
{

    // Accept command only if payload length is equal to 0
    if (msg_ptr->packet_in[msg_h_payload_len] == 0 )
    {

    	// update dsp flags
    	comm_flags.init &= ~DSP_FLAG_SEND_START_CMD_MASK;

    	// force alarm to continue with the initialization
    	rtc_alarm2000msecond_force_flag();

	}

}
#endif

/// @}

