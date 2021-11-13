/**
 * $Header
 * @file           fw_api.h
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

#ifndef FW_API_H_
#define FW_API_H_

/**************************** INCLUDES ***************************************/
#include "settings.h"
#include "comm/comm.h"

/**************************** FUNCTIONS **************************************/
void API_FW_ARM_version_req(msg_t *msg_ptr);
void API_FW_ARM_echo_req(msg_t *msg_ptr);
void API_FW_ARM_unknow_msg_id(msg_t *msg_ptr);

#endif /* FW_API_H_ */