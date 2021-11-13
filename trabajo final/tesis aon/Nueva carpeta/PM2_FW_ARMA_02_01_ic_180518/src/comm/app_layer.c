// $Header
/// @file			app_layer.c
/// @brief  		Communication application layer.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		27 December 2016
/// @details
/// @copyright
///

/// @addtogroup comm
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "app_layer.h"

// Local includes
#include "comm.h"
#include "../apis/fw_api.h"
#include "../apis/bootloader_api.h"
#include "../apis/unit_config_api.h"
#include "../apis/rtc_api.h"
#include "../apis/sdcard_api.h"
#include "../apis/dsp_api.h"

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

	if (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_DSP_ID)
		comm_bypass_msg( UART_DSP , msg_ptr );

	else if ( (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_RPI_ID) || (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_OTHERS_ID) )
		comm_bypass_msg( UART_RPI , msg_ptr );
	else
		switch (msg_ptr->packet_in[msg_h_msg_id])
		{
			// FW
			case API_FW_ARM_VERSION_REQ:                    API_FW_ARM_version_req( msg_ptr );                    break;
			case API_FW_ARM_ECHO_REQ:                       API_FW_ARM_echo_req( msg_ptr );                       break;
			case API_FW_ARM_UNIQUE_ID_REQ:                  API_FW_ARM_unique_id_req( msg_ptr );                  break;
			case API_FW_ARM_RESET_ID_REQ:                   API_FW_ARM_reset_req( msg_ptr );                      break;
			
			// Unit configuration and calibration
			case API_UNIT_CONFIG_ARM_UPDATE_REQ:            API_UNIT_CONFIG_ARM_update_req( msg_ptr );            break;
			case API_UNIT_CONFIG_ARM_SEND_REQ:              API_UNIT_CONFIG_ARM_send_req( msg_ptr );              break;
			case API_UNIT_CONFIG_ARM_SET_DEFAULT_REQ:       API_UNIT_CONFIG_ARM_set_default_req( msg_ptr );       break;
			case API_UNIT_CALIBRATION_ARM_UPDATE_REQ:       API_UNIT_CALIBRATION_ARM_update_req( msg_ptr );       break;
			case API_UNIT_CALIBRATION_ARM_SEND_REQ:         API_UNIT_CALIBRATION_ARM_send_req( msg_ptr );         break;
			case API_UNIT_CALIBRATION_ARM_SET_DEFAULT_REQ:  API_UNIT_CALIBRATION_ARM_set_default_req( msg_ptr );  break;

			// Rel Time Clock
			case API_RTC_ARM_SEND_REQ:                      API_RTC_ARM_send_req( msg_ptr );                      break;
			case API_RTC_ARM_UPDATE_REQ:                    API_RTC_ARM_update_req( msg_ptr );                    break;

			// SD-Card
			case API_SDCARD_DIR_OPEN_REQ:                   API_SDCARD_dir_open_req( msg_ptr );                   break;
			case API_SDCARD_DIR_ITEM_NEXT_REQ:              API_SDCARD_item_next_req( msg_ptr );                  break;
			case API_SDCARD_DIR_ITEM_NAME_REQ:              API_SDCARD_item_name_req( msg_ptr );                  break;
			case API_SDCARD_FILE_OPEN_REQ:                  API_SDCARD_file_open_req( msg_ptr );                  break;
			case API_SDCARD_FILE_READ_REQ:                  API_SDCARD_file_read_req( msg_ptr );                  break;
			case API_SDCARD_FILE_CLOSE_REQ:                 API_SDCARD_file_close_req( msg_ptr );                 break;
			case API_SDCARD_FILE_SIZE_REQ:                  API_SDCARD_file_size_req( msg_ptr );                  break;
			case API_SDCARD_FILE_AND_DIR_REMOVE_REQ:        API_SDCARD_file_and_dir_remove_req( msg_ptr );        break;
			case API_SDCARD_GET_MBYTES_REQ:                 API_SDCARD_get_mbytes_req( msg_ptr );	              break;

			// DSP Responses
			case API_FW_DSP_VERSION_RES:	                API_FW_DSP_version_res( msg_ptr );                    break;
			case API_FW_DSP_SEND_COMM_FLAG_RES:             API_FW_DSP_send_comm_flag_res( msg_ptr );             break;
			case API_RTC_DSP_UPDATE_RES:                    API_RTC_DSP_update_res( msg_ptr );                    break;
			case API_UNIT_CONFIG_DSP_UPDATE_RES:            API_UNIT_CONFIG_DSP_update_res( msg_ptr );            break;
			case API_UNIT_CALIBRATION_DSP_UPDATE_RES:       API_UNIT_CALIBRATION_DSP_update_res( msg_ptr );       break;
			case API_CTRL_DSP_TREND_150CYCLES_RES:          API_CTRL_DSP_trend_150cycles_res( msg_ptr );          break;
			case API_CTRL_DSP_TREND_5MINUTES_RES:           API_CTRL_DSP_trend_5minutes_res( msg_ptr );           break;
			case API_CTRL_DSP_EVENT_BUFFER_RES:             API_CTRL_DSP_event_buffer_res( msg_ptr );             break;
			case API_CTRL_DSP_EVENT_DET_RES:                API_CTRL_DSP_event_det_res( msg_ptr );                break;
			
			// ARM - Bootloader
			case API_BOOT_ARM_START_UPLOAD_REQ:             API_BOOT_ARM_start_upload_req( msg_ptr );             break;
			case API_BOOT_ARM_UPLOAD_BLOCK_REQ:             API_BOOT_ARM_upload_block_req( msg_ptr );             break;
			case API_BOOT_ARM_INSTALL_IMAGE_REQ:            API_BOOT_ARM_install_image_req( msg_ptr );            break;
			
			// Unknown command
			default:                                        API_FW_ARM_unknow_msg_id( msg_ptr );	              break;

		}

}

/// @}
