/** $Header
  * @file            app_layer.h
  * @brief           Communications app layer function.
  * @details
  * @author          Agustin Aon
  * @version         0.1
  * @date            21 July 2020
  * @details
  * @copyright
  */

#ifndef APP_LAYER_H_
#define APP_LAYER_H_


/**************************** INCLUDES ***************************************/
#include "settings.h"
#include "comm.h"


/**************************** MACROS *****************************************/

// ARM - Firmware
#define API_FW_ARM_VERSION_REQ                    (0x02)
#define API_FW_ARM_VERSION_RES                    (0x03)
#define API_FW_ARM_ECHO_REQ                       (0x04)
#define API_FW_ARM_ECHO_RES                       (0x05)
#define API_FW_ARM_UNKNOW_MSG_ID_RES              (0x0F)

// ARM - Control
#define API_CONTROL_START_REQ                     (0x10)
#define API_CONTROL_START_RES                     (0x11)
#define API_CONTROL_STOP_REQ                      (0x12)
#define API_CONTROL_STOP_RES                      (0x13)
#define API_CONTROL_SET_SIGNAL_PARAMS_REQ         (0x14)
#define API_CONTROL_SET_SIGNAL_PARAMS_RES         (0x15)
#define API_CONTROL_TREND_REQ                     (0x16)
#define API_CONTROL_TREND_RES                     (0x17)
#define API_CONTROL_SET_CUSTOM_LUT_REQ            (0x18)
#define API_CONTROL_SET_CUSTOM_LUT_RES            (0x19)
#define API_CONTROL_USE_CUSTOM_LUT_REQ            (0x1A)
#define API_CONTROL_USE_CUSTOM_LUT_RES            (0x1B)
#define API_CONTROL_TELEMETRY_REQ                 (0x1C)
#define API_CONTROL_TELEMETRY_RES                 (0x1D)
#define API_CONTROL_SET_PWM_DUTY_REQ              (0x1E)
#define API_CONTROL_SET_PWM_DUTY_RES              (0x1F)


/**************************** FUNCTIONS **************************************/
void decode_incoming_comms_packet(msg_t *msg_ptr);


#endif /* APP_LAYER_H_ */