/** $Header
 * @file            control.h
 * @brief           control function.
 * @details
 * @author          Agustin Aon
 * @version         0.1
 * @date            21 July 2020
 * @details
 * @copyright
 */

#ifndef CONTROL_H_
#define CONTROL_H_

/**************************** INCLUDES ***************************************/
#include "settings.h"
#include "apps_dsp/trend.h"
#include "apps_dsp/signal_processing.h"


/**************************** FUNCTIONS **************************************/
void control_init(void);
void control_update(void);
void control_background(void);

void control_enable(uint8_t enable_b);
uint8_t control_is_enable(void);

void control_enable_ch(uint8_t ch);
void control_disable_ch(uint8_t ch);

uint8_t control_get_enabled_ch(void);
uint8_t control_get_num_enabled_ch(void);

signal_params_t *control_get_params(void);
void control_set_params(uint16_t amp, uint16_t freq);

trend_result_t *control_get_trend(void);

void control_check_param_change(uint8_t *flags_ptr, signal_params_t *sp);

uint16_t *control_get_pwm_duty_buf(void);
void control_set_pwm_duty(uint8_t duty);

void control_use_custom_lut(uint8_t use);

#endif /* CONTROL_H_ */