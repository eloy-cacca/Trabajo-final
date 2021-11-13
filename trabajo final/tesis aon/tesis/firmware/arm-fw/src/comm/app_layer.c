/**
 * $Header
 * @file           app_layer.c
 * @brief          Communications app layer function.
 * @details
 * @author         Agustin Aon
 * @version        0.01
 * @date           21 July 2020
 * @details
 * @copyright
 *
 * @addtogroup main
 * @{
 */


/**************************** INCLUDES ***************************************/
#include "app_layer.h"
#include "api/fw_api.h"
#include "api/control_api.h"


/**************************** GLOBAL FUNCTIONS *******************************/
void decode_incoming_comms_packet( msg_t *msg_ptr )
{
    switch (msg_ptr->packet_in[msg_h_msg_id])
    {
        // FW
        case API_FW_ARM_VERSION_REQ:                    API_FW_ARM_version_req(msg_ptr);                        break;
        case API_FW_ARM_ECHO_REQ:                       API_FW_ARM_echo_req(msg_ptr);                           break;

        // CONTROL
        case API_CONTROL_START_REQ:                     API_CONTROL_start_req(msg_ptr);                         break;
        case API_CONTROL_STOP_REQ:                      API_CONTROL_stop_req(msg_ptr);                          break;
        case API_CONTROL_SET_SIGNAL_PARAMS_REQ:         API_CONTROL_set_signal_params_req(msg_ptr);             break;
        case API_CONTROL_TREND_REQ:                     API_CONTROL_trend_req(msg_ptr);                         break;
        case API_CONTROL_SET_CUSTOM_LUT_REQ:            API_CONTROL_set_custom_lut_req(msg_ptr);                break;
        case API_CONTROL_USE_CUSTOM_LUT_REQ:            API_CONTROL_use_custom_lut_req(msg_ptr);                break;
        case API_CONTROL_TELEMETRY_REQ:                 API_CONTROL_telemetry_req(msg_ptr);                     break;
        case API_CONTROL_SET_PWM_DUTY_REQ:              API_CONTROL_set_pwm_duty_req(msg_ptr);                  break;

        // Unknown command
        default:                                        API_FW_ARM_unknow_msg_id(msg_ptr);                      break;
    }
}
