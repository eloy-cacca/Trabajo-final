// $Header
/// @file			unit_config_api.c
/// @brief  		Unit config application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		12 August 2016
/// @details
/// @copyright
///

/// @addtogroup apis
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "unit_config_api.h"

// local includes
#include "../apps/unit_config.h"
#include "../utils/byte_string_operations.h"
#include "../apps_comm/app_layer.h"
#include "../apps_comm/comm.h"
#include "../apps/rtc.h"
#include "../apps/control.h"

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
void API_UNIT_CONFIG_DSP_update_req( msg_t * msg_ptr )
{

	uint8_t response;
	static unit_config_t temp;

    // Accept command only if payload length is equal to unit config length
    if (msg_ptr->packet_in[msg_h_payload_len] == (sizeof(unit_config_t)<<1) )
    {

    	// save unit config into a temporal register
    	copy_content_form_8bits_to_16bits( (uint16_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
			                               (uint16_t *)&temp ,
			                               msg_ptr->packet_in[msg_h_payload_len] );

    	// Set unit configuration with custom parameters
    	unit_config_set_custom(&temp);

    	// clear comm flags
    	system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_UPDATE_UCONFIG;

    	// update trend flag
    	*ctrl_trend_flag_temp_ptr() |= SETTINGS_TREND_FLAGS_UNIT_CONFIG_UPDATE;

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


/// @brief Get unit configuration.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CONFIG_DSP_send_req( msg_t * msg_ptr )
{

	comm_send_message( msg_ptr->sci_ptr ,
                       msg_ptr->packet_in[msg_h_src_id] ,
	                   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	                   sizeof(unit_config_t)<<1, //sizeof return the size in 16 bits words
	                   (uint8_t *)&unit_config
	                   );

}

/// @brief Set unit configuration to default.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CONFIG_DSP_set_default_req( msg_t * msg_ptr )
{

    uint8_t response;

    // Accept command only if payload length is equal to 0
    if (msg_ptr->packet_in[msg_h_payload_len] == 0)
    {

    	// set unit configuration to default
    	unit_config_set_default();

    	// clear comm flags
    	system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_UPDATE_UCONFIG;

    	// update trend flag
    	*ctrl_trend_flag_temp_ptr() |= SETTINGS_TREND_FLAGS_UNIT_CONFIG_UPDATE;

   		// response
		response = 0x00;

    }
    else
    {

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





/// @brief Set unit calibration with custom data.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CALIBRATION_DSP_update_req( msg_t * msg_ptr )
{

	uint8_t response;
	static unit_calibration_t temp;

    // Accept command only if payload length is equal to unit config length
    if (msg_ptr->packet_in[msg_h_payload_len] == (sizeof(unit_calibration_t)<<1) )
    {

    	// save unit config into a temporal register
    	copy_content_form_8bits_to_16bits( (uint16_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
			                               (uint16_t *)&temp ,
			                               msg_ptr->packet_in[msg_h_payload_len] );

    	// Set unit configuration with custom parameters
    	unit_calibration_set_custom(&temp);

    	// clear comm flags
    	system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_UPDATE_UCALIBRATION;

    	// update trend flag
    	*ctrl_trend_flag_temp_ptr() |= SETTINGS_TREND_FLAGS_UNIT_CALIBRATION_UPDATE;

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


/// @brief Get unit calibration.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CALIBRATION_DSP_send_req( msg_t * msg_ptr )
{

	comm_send_message( msg_ptr->sci_ptr ,
                       msg_ptr->packet_in[msg_h_src_id] ,
	                   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	                   sizeof(unit_calibration_t)<<1, //sizeof return the size in 16 bits words
	                   (uint8_t *)&unit_calibration
	                   );

}

/// @brief Set unit calibration to default.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UNIT_CALIBRATION_DSP_set_default_req( msg_t * msg_ptr )
{

    uint8_t response;

    // Accept command only if payload length is equal to 0
    if (msg_ptr->packet_in[msg_h_payload_len] == 0)
    {

    	// set unit configuration to default
    	unit_calibration_set_default();

    	// clear comm flags
    	system_flags.comm_dsp &= ~SETTINGS_COMM_RPI_DSP_UPDATE_UCALIBRATION;

    	// update trend flag
    	*ctrl_trend_flag_temp_ptr() |= SETTINGS_TREND_FLAGS_UNIT_CALIBRATION_UPDATE;

   		// response
		response = 0x00;

    }
    else
    {

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

#ifdef DELETE
/// @brief Unit Config update request.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_UC_unit_config_update_req(void) {

	comm_send_message( &SCIB ,
			           SETTINGS_ARM_ID ,
			           API_UC_UNIT_CONFIG_UPDATE_REQ ,
			           0 ,
					   0
					   );
}
#endif

/// @brief Unit Config update response
///        This is a response message, so no response is needed.
///
/// @param[in] msp_ptr Pointer to incoming message
///
/*void API_UC_unit_config_update_res( msg_t * msg_ptr ) {

	unit_config_t temp;

    // Accept command only if payload length is equal to 0
    if (msg_ptr->packet_in[msg_h_payload_len] == (sizeof(unit_config_t)<<1) )
    {

    	// save rtc into a temporal register
    	copy_content_form_8bits_to_16bits( (uint16_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
			                               (uint16_t *)&temp ,
			                               msg_ptr->packet_in[msg_h_payload_len] );

    	// Set unit configuration with custom parameters
    	unit_config_set_custom(&temp);

    	// update dsp flags
    	comm_flags.init &= ~DSP_FLAG_UC_REQUEST_MASK;

    	// force alarm to continue with the initialization
    	rtc_alarm2000msecond_force_flag();

	}

}*/

/*void API_UC_unit_config_force_update( msg_t * msg_ptr ) {

	comm_flags.init = DSP_FLAG_RTC_REQUEST_MASK | DSP_FLAG_UC_REQUEST_MASK;

}*/

/// @}

