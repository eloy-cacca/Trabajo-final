/**
 * $Header
 * @file           fw_api.c
 * @brief          Firmware api.
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
#include "fw_api.h"
#include "comm/app_layer.h"

/**************************** GLOBAL FUNCTIONS *******************************/

/**
 * @brief Response to FW version request.
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_FW_ARM_version_req(msg_t *msg_ptr)
{

  // by default size = 2 + 12 + 9
  const uint8_t fw_version[2 + sizeof(__DATE__) + sizeof(__TIME__)] = {
      SETTINGS_ARM_FW_VERSION, SETTINGS_ARM_FW_VERSION_ENG_BUILD,
      __DATE__[0], __DATE__[1], __DATE__[2], __DATE__[3], __DATE__[4], __DATE__[5], __DATE__[6], __DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10], __DATE__[11],
      __TIME__[0], __TIME__[1], __TIME__[2], __TIME__[3], __TIME__[4], __TIME__[5], __TIME__[6], __TIME__[7], __TIME__[8]};

  comm_send_msg(msg_ptr->uart_ptr,
                msg_ptr->packet_in[msg_h_src_id],
                msg_ptr->packet_in[msg_h_msg_id] + 1,
                sizeof(fw_version),
                (uint8_t *)&fw_version[0]);
}

/**
 * @brief Send echo back to source.
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_FW_ARM_echo_req(msg_t *msg_ptr)
{

  comm_send_msg(msg_ptr->uart_ptr,
                msg_ptr->packet_in[msg_h_src_id],
                msg_ptr->packet_in[msg_h_msg_id] + 1,
                msg_ptr->packet_in[msg_h_payload_len],
                (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte]);
}

/**
 * @brief Msg ID unknown. Received ID is send in the payload.
 * @param[in] msp_ptr Pointer to incoming message
 */
void API_FW_ARM_unknow_msg_id(msg_t *msg_ptr)
{
  comm_send_msg(msg_ptr->uart_ptr,
                msg_ptr->packet_in[msg_h_src_id],
                API_FW_ARM_UNKNOW_MSG_ID_RES,
                1,
                (uint8_t *)&msg_ptr->packet_in[msg_h_msg_id]);
}