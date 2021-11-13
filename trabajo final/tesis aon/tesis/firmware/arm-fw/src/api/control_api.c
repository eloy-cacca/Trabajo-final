/**
 * $Header
 * @file           control_api.c
 * @brief          Control api.
 * @details
 * @author         Agustin Aon
 * @version        0.01
 * @date           21 September 2020
 * @details
 * @copyright
 *
 * @addtogroup main
 * @{
 */

/**************************** INCLUDES ***************************************/
// Specific includes
#include "control_api.h"
#include "apps/control.h"
#include "apps_dsp/trend.h"
#include "utils/byte_string_operations.h"
#include "utils/lut.h"

/**************************** GLOBAL FUNCTIONS *******************************/

/**
 * @brief   Response to CONTROL start request.
 * @details Enables ADC, PWM, control_update and control_background modules.
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_CONTROL_start_req(msg_t *msg_ptr)
{

    // enable control
    control_enable(1);

    // return echo
    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  msg_ptr->packet_in[msg_h_payload_len],
                  (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte]);
}

/**
 * @brief   Response to CONTROL stop request.
 * @details Disables ADC, PWM, control_update and control_background modules.
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_CONTROL_stop_req(msg_t *msg_ptr)
{

    // disable control
    control_enable(0);

    // return echo
    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  msg_ptr->packet_in[msg_h_payload_len],
                  (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte]);
}

/**
 * @brief Response to SET SIGNAL PARAMETERS request.
 * @details Sets the parameters of the signal (like amplitude and frequency)
 *          to the desired user value. The payload should be sent this way:
 *              1st byte: amplitude in RMS (if default LUT, else Vp)
 *              2nd byte: frequency in Hz
 *          Only integer values allowed.
 *          Sends back:
 *              0 if no error
 *              1 if param num wrong
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_CONTROL_set_signal_params_req(msg_t *msg_ptr)
{

    uint16_t amp;
    uint16_t freq;
    uint8_t ret_err;

    // Check if number of parameters is ok
    if (msg_ptr->packet_in[msg_h_payload_len] != 4)
    {
        ret_err = 1;
    }
    else
    {
        ret_err = 0;
        amp = msg_ptr->packet_in[msg_h_payload_1st_byte] + (msg_ptr->packet_in[msg_h_payload_1st_byte + 1] << 8);
        freq = msg_ptr->packet_in[msg_h_payload_1st_byte + 2] + (msg_ptr->packet_in[msg_h_payload_1st_byte + 3] << 8);

        control_set_params(amp, freq);
    }
    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  1,
                  (uint8_t *)&ret_err);
}

/**
 * @brief   Response to TREND request.
 * @details Sends back trends calculated so far.
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_CONTROL_trend_req(msg_t *msg_ptr)
{
    trend_result_t *trend;

    // get trend from control.c
    trend = control_get_trend();

    // send trend struct by parts, given msg length limitation
    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  sizeof(trend->flags),
                  (uint8_t *)&trend->flags);

    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  sizeof(trend->params),
                  (uint8_t *)&trend->params);

    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  sizeof(trend->ch[0]),
                  (uint8_t *)&trend->ch[0]);

    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  sizeof(trend->ch[1]),
                  (uint8_t *)&trend->ch[1]);

    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  sizeof(trend->ch[2]),
                  (uint8_t *)&trend->ch[2]);

    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  sizeof(trend->ch[3]),
                  (uint8_t *)&trend->ch[3]);
}

/**
 * @brief Change LUT custom table.
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_CONTROL_set_custom_lut_req(msg_t *msg_ptr)
{
    uint8_t ret_err = 0;

    if (msg_ptr->rx_counter != (LUT_128_N * sizeof(int32_t)))
    {
        ret_err = 1;
    }
    else
    {
        set_custom_lut((int32_t *)&msg_ptr->packet_in);
    }

    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  1,
                  &ret_err);
} //TODO: test this

/**
 * @brief Set LUT table usage.
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_CONTROL_use_custom_lut_req(msg_t *msg_ptr)
{
    uint8_t ret_err = 0;

    if ((msg_ptr->rx_counter != 1) || (msg_ptr->packet_in[0] != 1) || (msg_ptr->packet_in[0] != 0))
    {
        ret_err = 1;
    }
    else
    {
        control_use_custom_lut(msg_ptr->packet_in[0]);
    }

    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  1,
                  &ret_err);
} //TODO: test this

/**
 * @brief Send telemetry data.
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_CONTROL_telemetry_req(msg_t *msg_ptr)
{
    signal_params_t *params = control_get_params();

    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  sizeof(signal_params_t),
                  (uint8_t *)params);
}

/**
 * @brief Set PWM duty request.
 * @param[in] msg_ptr Pointer to incoming message
 */
void API_CONTROL_set_pwm_duty_req(msg_t *msg_ptr)
{
    uint8_t ret_err = 0;
    uint8_t duty = msg_ptr->packet_in[msg_h_payload_1st_byte];

    if (duty > 100)
        ret_err = 1;
    else
        control_set_pwm_duty(duty);

    comm_send_msg(msg_ptr->uart_ptr,
                  msg_ptr->packet_in[msg_h_src_id],
                  msg_ptr->packet_in[msg_h_msg_id] + 1,
                  1,
                  &ret_err);
}
