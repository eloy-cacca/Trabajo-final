/**
 * $Header
 * \file           fw_api.h
 * \brief          Firmware api.
 * \details
 * \author         Agustin Aon
 * \version        0.01
 * \date           21 July 2020
 * \details
 * \copyright
 *
 * \addtogroup main
 * \{
 */

#ifndef CONTROL_API_H_
#define CONTROL_API_H_

/**************************** INCLUDES ***************************************/
#include "settings.h"
#include "comm/comm.h"

/**************************** FUNCTIONS **************************************/
void API_CONTROL_start_req(msg_t *msg_ptr);
void API_CONTROL_stop_req(msg_t *msg_ptr);
void API_CONTROL_set_signal_params_req(msg_t *msg_ptr);
void API_CONTROL_trend_req(msg_t *msg_ptr);
void API_CONTROL_set_custom_lut_req(msg_t *msg_ptr);
void API_CONTROL_use_custom_lut_req(msg_t *msg_ptr);
void API_CONTROL_telemetry_req(msg_t *msg_ptr);
void API_CONTROL_set_pwm_duty_req(msg_t *msg_ptr);

#endif /* CONTROL_API_H_ */