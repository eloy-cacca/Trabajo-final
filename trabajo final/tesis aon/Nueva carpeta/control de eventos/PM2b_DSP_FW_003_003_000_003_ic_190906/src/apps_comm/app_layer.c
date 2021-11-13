// $Header
/// @file			app_layer.c
/// @brief  		Communication application layer.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		05 August 2016
/// @details
/// @copyright
///

/// @addtogroup apps
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "app_layer.h"

// Local includes
#include "comm.h"
#include "../apis/rtc_api.h"
#include "../apis/fw_api.h"
#include "../apis/unit_config_api.h"
#include "../apis/control_api.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Decode incoming message from SCI.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void decode_incoming_comms_packet( msg_t * msg_ptr )
{

/*	if (msg_ptr->packet_in[msg_h_des_id] == DEVICE_ID_FPGA)
		comm_send_request( msg_ptr , FPGA_UART );

	else if (msg_ptr->packet_in[msg_h_des_id] == DEVICE_ID_VFP)
		comm_send_request( msg_ptr , VFP_UART );

	else*/
		switch (msg_ptr->packet_in[msg_h_msg_id])
		{
			// FW
			case API_FW_DSP_VERSION_REQ:                    API_FW_version_req( msg_ptr );                        break;
			case API_FW_DSP_ECHO_REQ:                       API_FW_echo_req( msg_ptr );                           break;
			case API_FW_DSP_SEND_COMM_FLAG_REQ:             API_FW_dsp_send_comm_flag_req( msg_ptr );             break;
		
			// RTC
			case API_RTC_DSP_UPDATE_REQ:                    API_RTC_DSP_update_req( msg_ptr );                    break;
			case API_RTC_DSP_SEND_REQ:                      API_RTC_DSP_send_req( msg_ptr );                      break;

			// Unit configuration
			case API_UNIT_CONFIG_DSP_UPDATE_REQ:            API_UNIT_CONFIG_DSP_update_req ( msg_ptr );           break;
			case API_UNIT_CONFIG_DSP_SEND_REQ:              API_UNIT_CONFIG_DSP_send_req ( msg_ptr );             break;
			case API_UNIT_CONFIG_DSP_SET_DEFAULT_REQ:       API_UNIT_CONFIG_DSP_set_default_req ( msg_ptr );      break;
			case API_UNIT_CALIBRATION_DSP_UPDATE_REQ:       API_UNIT_CALIBRATION_DSP_update_req ( msg_ptr );      break;
			case API_UNIT_CALIBRATION_DSP_SEND_REQ:         API_UNIT_CALIBRATION_DSP_send_req ( msg_ptr );        break;
			case API_UNIT_CALIBRATION_DSP_SET_DEFAULT_REQ:  API_UNIT_CALIBRATION_DSP_set_default_req ( msg_ptr ); break;

			// Processing
			case API_CTRL_DSP_TREND_150CYCLES_REQ:          API_CTRL_trend_150cycles_req( msg_ptr );              break;
			case API_CTRL_DSP_TREND_5MINUTES_REQ:           API_CTRL_trend_5minutes_req( msg_ptr );               break;
			case API_CTRL_DSP_EVENT_BUFFER_REQ:             API_CTRL_event_buffer_req( msg_ptr );                 break;
			case API_CTRL_DSP_EVENT_DET_REQ:                API_CTRL_event_det_req( msg_ptr );                    break;
			case API_CTRL_DSP_TELEMETRIA_REQ:               API_CTRL_telemetria_req( msg_ptr );                   break;

			// Unknown command
			default:                                        API_FW_unknow_msg_id ( msg_ptr );		              break;
		}

}

/// @}
