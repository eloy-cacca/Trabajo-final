// $Header
/// @file			unit_config_api.c
/// @brief  		Unit config application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		28 December 2016
/// @details
/// @copyright
///

/// @addtogroup apis
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "unit_config_api.h"

// local includes
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

/// @brief Set unit configuration with custom data.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CONFIG_ARM_update_req( msg_t * msg_ptr )
{

	uint8_t response;
	static unit_config_t temp;

    // Accept command only if payload length is equal to unit config length
    if (msg_ptr->packet_in[msg_h_payload_len] == sizeof(unit_config_t) )
    {

    	// save unit config into a temporal register
    	copy_content( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
			          (uint8_t *)&temp ,
                      msg_ptr->packet_in[msg_h_payload_len] );

    	// Set unit configuration with custom parameters
    	unit_config_set_custom(&temp);

		// set flag to force DSP update in background
		comm_flags |= SETTINGS_COMM_FLAG_UNIT_CONFIG_REQUEST_MASK;

		// if SD-Card is ready, replace config.txt data
		if (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY)
			sdcard_save_unit_config();

   		// response
		response = 0x00;

    } else {

    	// response
	    response = 0x01;

	}

	comm_send_msg( msg_ptr->uart_ptr ,
                   msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               1 ,
	               &response);

}


/// @brief Get unit configuration
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CONFIG_ARM_send_req( msg_t * msg_ptr )
{

	comm_send_msg( msg_ptr->uart_ptr ,
                   msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               sizeof(unit_config_t),
	               (uint8_t *)&unit_config);

}

/// @brief Set unit configuration to default.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CONFIG_ARM_set_default_req( msg_t * msg_ptr )
{

	uint8_t response;

	// Accept command only if payload length is equal to 0
	if (msg_ptr->packet_in[msg_h_payload_len] == 0)
	{

		// set unit configuration to default
		unit_config_set_default();

		// set flag to force DSP update in background
		comm_flags |= SETTINGS_COMM_FLAG_UNIT_CONFIG_REQUEST_MASK;

		// if SD-Card is ready, replace config.txt data
		if (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY)
			sdcard_save_unit_config();

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

/// @brief Set unit configuration with custom data.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CALIBRATION_ARM_update_req( msg_t * msg_ptr )
{

	uint8_t response;
	static unit_calibration_t temp;

	// Accept command only if payload length is equal to unit config length
	if (msg_ptr->packet_in[msg_h_payload_len] == sizeof(unit_calibration_t) )
	{

		// save unit config into a temporal register
		copy_content( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
		              (uint8_t *)&temp ,
	                  msg_ptr->packet_in[msg_h_payload_len] );

		// Set unit configuration with custom parameters
		unit_calibration_set_custom(&temp);

		// set flag to force DSP update in background
		comm_flags |= SETTINGS_COMM_FLAG_UNIT_CALIBRATION_REQUEST_MASK;

		// if SD-Card is ready, replace config.txt data
		if (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY)
			sdcard_save_unit_calibration();

		// response
		response = 0x00;

	} else {

		// response
		response = 0x01;

	}

	comm_send_msg( msg_ptr->uart_ptr ,
                   msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               1 ,
	               &response);

}


/// @brief Get unit configuration.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CALIBRATION_ARM_send_req( msg_t * msg_ptr )
{

	comm_send_msg( msg_ptr->uart_ptr ,
	               msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               sizeof(unit_calibration_t),
	               (uint8_t *)&unit_calibration);

}

/// @brief Set unit configuration to default.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CALIBRATION_ARM_set_default_req( msg_t * msg_ptr )
{

	uint8_t response;

	// Accept command only if payload length is equal to 0
	if (msg_ptr->packet_in[msg_h_payload_len] == 0)
	{

		// set unit configuration to default
		unit_calibration_set_default();

		// set flag to force DSP update in background
		comm_flags |= SETTINGS_COMM_FLAG_UNIT_CALIBRATION_REQUEST_MASK;

		// if SD-Card is ready, replace config.txt data
		if (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY)
			sdcard_save_unit_calibration();

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

