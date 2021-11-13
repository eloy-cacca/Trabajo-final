/**
 * $Header
 * @file           control.c
 * @brief          control function.
 * @details
 * @author         Agustin Aon
 * @version        0.1
 * @date           21 July 2020
 * @details
 * @copyright
 *
 * @addtogroup apps
 * @{
 */

/**************************** INCLUDES ***************************************/
#include "control.h"
#include "apps_dsp/loop.h"
#include "apps_dsp/index.h"
#include "apps_dsp/signal_adc.h"
#include "utils/byte_string_operations.h"
#include "system.h"
#include "signal_pwm.h"
#include "utils/lut.h" //FIXME debug


/**************************** LOCAL VARIABLES ********************************/
static int32_t          signals[SETTINGS_CH_TOTAL_NUM];  // sensors data, Q12
static int32_t          control_signal;
static sp_t             processed_signals;
static index_accum_t    index_accum[2];
static index_result_t   index_result;
static trend_accum_t    trend_accum;
static trend_result_t   trend_result;
static signal_params_t  signal_params;
static uint8_t          pwm_duty = 100;
static uint16_t         pwm_duty_buf[SETTINGS_PWM_BUF_LEN] = {0};


/**************************** LOCAL FUNCTIONS ********************************/

/** @brief Set PWM buffer from value. */
static void set_pwm_duty_buf(uint8_t duty)
{
    uint8_t i;
    for (i = 0; i < SETTINGS_PWM_BUF_LEN; i++)
    {
        pwm_duty_buf[i] = (uint16_t) duty;
    }
}

/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief Control module initialization. */
void control_init(void)
{
    // clear variables
    memset((void *)&signals, 0, sizeof(signals));
    memset((void *)&processed_signals, 0, sizeof(processed_signals));
    memset((void *)&signal_params, 0, sizeof(signal_params));

    // index
    memset((void *)&index_accum, 0, sizeof(index_accum));
    memset((void *)&index_result, 0, sizeof(index_result));

    // trend
    trend_reset_accum(&trend_accum);
    trend_reset_result(&trend_result);

    // pwm buf
    memset((void *)&pwm_duty_buf[0], 0, sizeof(pwm_duty_buf));
}

/** @brief Control module routine. */
void control_update(void)
{
    /***************** ADC ******************/
    signal_adc_update(&signals[0]);

    /************* CONTROLLER ***************/
    //loop_update(&signals[0], &control_signal);
    signal_process(&signals[0], &processed_signals);

    /**************** PWM ******************/
    set_pwm_duty_buf(pwm_duty);

    /*********** INDEX & TREND **************/
    static uint8_t id = 0;

    // Select accumulator, so that it's always accumulating data
    if (index_accum[0].accum == SETTINGS_INDEX_ACCUM_MAX)
        id = 1;
    else if (index_accum[1].accum == SETTINGS_INDEX_ACCUM_MAX)
        id = 0;

    // Update index accumulators.
    // It should never execute if accum is full, but just in case.
    if (index_accum[id].accum < SETTINGS_INDEX_ACCUM_MAX)
        index_update_accum(&processed_signals, &index_accum[id]);
}

/** @brief Control background routines */
void control_background(void)
{
    // Check if control is enabled
    if (!signal_params.enabled)
        return;

    // Update index result and trend accum
    if (index_accum[0].accum == SETTINGS_INDEX_ACCUM_MAX)
    {
        index_update_result(&index_accum[0], &index_result);
        trend_update_accum(&index_result, &trend_accum);
    }
    else if (index_accum[1].accum == SETTINGS_INDEX_ACCUM_MAX)
    {
        index_update_result(&index_accum[1], &index_result);
        trend_update_accum(&index_result, &trend_accum);
    }

    // Update trend result if condition is met
    if (trend_accum.accum == SETTINGS_TREND_MAX)
    {
        trend_update_result(&trend_accum, &trend_result);
        trend_reset_accum(&trend_accum);
    }
}

/** @brief Set signal parameters */
void control_set_params(uint16_t amp, uint16_t fL)
{
    signal_params.amp = amp;
    signal_params.fL = fL;

    // Reset mDFT buffer
    memset((void *)&processed_signals, 0, sizeof(processed_signals));
}

/** @brief Enable/disable control routines */
void control_enable(uint8_t enable_b)
{
    control_init();
    signal_params.enabled = enable_b;
    if (enable_b)
    {
        led_red_on();
        signal_pwm_enable(true);
        drv1_on();
        drv2_on();
    }
    else
    {
        led_red_off();
        signal_pwm_enable(false);
        drv1_off();
        drv2_off();
    }
}

/** @brief Send trends calculated so far */
trend_result_t *control_get_trend()
{
    return &trend_result;
}

/** @brief Returns true or false depending if control is enabled. */
uint8_t control_is_enable(void)
{
    return signal_params.enabled;
}

/** @brief Return signal params */
signal_params_t *control_get_params()
{
    return &signal_params;
}

/** @brief Check if the parameters changed and raised the appropriate flags. */
void control_check_param_change(uint8_t *flags_ptr, signal_params_t *sp)
{
    if (sp->fL != signal_params.fL)
        SET_FLAG(*flags_ptr, SETTINGS_TRENDS_FREQ_CHANGED);

    if (sp->amp != signal_params.amp)
        SET_FLAG(*flags_ptr, SETTINGS_TRENDS_AMP_CHANGED);

    if (sp->enabled != signal_params.enabled)
        SET_FLAG(*flags_ptr, SETTINGS_TRENDS_ENABLED_CHANGED);

    if (sp->custom_waveform_b != signal_params.custom_waveform_b)
        SET_FLAG(*flags_ptr, SETTINGS_TRENDS_CUSTOM_WAVEFORM_CHANGED);
}

/** @brief Return pointer to PWM duty buffer. */
uint16_t *control_get_pwm_duty_buf()
{
    return &pwm_duty_buf[0];
}

/** @brief Set PWM duty. */
void control_set_pwm_duty(uint8_t duty)
{
    pwm_duty = duty;
}

/** @brief Set custom LUT usage. */
void control_use_custom_lut(uint8_t use)
{
    signal_params.custom_waveform_b = use;
}
