/**
 * @file           pwm.c
 * @brief          pwm function.
 * @details
 * @author         Agustin Aon
 * @version        0.1
 * @date           11 April 2021
 * @details
 * @copyright
 *
 * @addtogroup apps
 * @{
 */

/**************************** INCLUDES ***************************************/
#include "signal_pwm.h"

/**************************** LOCAL VARIABLES ********************************/
static pwm_channel_t sync_channel;

/**************************** MACROS *****************************************/

/**
 * Lo que entendi de estos valores:
 *   FREQ: es la frecuencia a la que queremos que se actualice el PWM
 *   PERIOD: la precision que queremos tener en el duty
 *
 * Entonces el clock del PWM es FREQ * PERIOD.
 */
#define SETTINGS_PWM_FREQ                       (10E3)     // 100kHz
#define SETTINGS_PWM_PERIOD                     (100)       // prescaler para el clock

#define INIT_DUTY_VALUE                         (0)
#define INIT_DEAD_TIME                          (5)

/**
 * Esto vendria a ser la cantidad de periodos (no clocks) en la que
 * se actualiza el duty y demas valores, porque no es instantaneo.
 */
#define SYNC_UPDATE_PERIOD                    (1)           // maximo es 15 (PWM_SCUP_UPR_Msk)

// Pin configurations
#define SETTINGS_PWM_ADC_CHANNEL       (PWM_CHANNEL_1)
#define SETTINGS_PWM_ADC_H_PIN         (PIO_PA24_IDX)
#define SETTINGS_PWM_ADC_H_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)

// Driver 1 es U10
#define SETTINGS_PWM_DRV1_CHANNEL      (PWM_CHANNEL_2)
#define SETTINGS_PWM_DRV1_H_PIN        (PIO_PA2_IDX)
#define SETTINGS_PWM_DRV1_L_PIN        (PIO_PA30_IDX)
#define SETTINGS_PWM_DRV1_H_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
#define SETTINGS_PWM_DRV1_L_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)

// Driver 2 es U3
#define SETTINGS_PWM_DRV2_CHANNEL      (PWM_CHANNEL_0)
#define SETTINGS_PWM_DRV2_H_PIN        (PIO_PB5_IDX)        // estos pines se usan con la
#define SETTINGS_PWM_DRV2_L_PIN        (PIO_PA0_IDX)        // polaridad invertida
#define SETTINGS_PWM_DRV2_H_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)
#define SETTINGS_PWM_DRV2_L_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)


/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief PWM initialization routine. */
void signal_pwm_init()
{
    // Set GPIO
    REG_CCFG_SYSIO |= CCFG_SYSIO_SYSIO5;        // Change TDO to PB5

    //gpio_configure_pin(SETTINGS_PWM_ADC_H_PIN, SETTINGS_PWM_ADC_H_FLAGS);
    gpio_configure_pin(SETTINGS_PWM_DRV1_H_PIN, SETTINGS_PWM_DRV1_H_FLAGS);
    gpio_configure_pin(SETTINGS_PWM_DRV1_L_PIN , SETTINGS_PWM_DRV1_L_FLAGS);
    gpio_configure_pin(SETTINGS_PWM_DRV2_H_PIN, SETTINGS_PWM_DRV2_H_FLAGS);
    gpio_configure_pin(SETTINGS_PWM_DRV2_L_PIN, SETTINGS_PWM_DRV2_L_FLAGS);

    /* Enable PWM peripheral clock */
    pmc_enable_periph_clk(ID_PWM);

    /* Disable PWM channels */
    //pwm_channel_disable(PWM, SETTINGS_PWM_ADC_CHANNEL);
    pwm_channel_disable(PWM, SETTINGS_PWM_DRV1_CHANNEL);
    pwm_channel_disable(PWM, SETTINGS_PWM_DRV2_CHANNEL);

    /* Set PWM clock A as SETTINGS_PWM_FREQ*PERIOD_VALUE (clock B is not used) */
    pwm_clock_t clock_setting = {
        .ul_clka = SETTINGS_PWM_FREQ * SETTINGS_PWM_PERIOD,
        .ul_clkb = 0,
        .ul_mck = sysclk_get_cpu_hz()
    };

    pwm_init(PWM, &clock_setting);

    /* Initialize PWM synchronous channels */
    sync_channel.alignment = PWM_ALIGN_LEFT,
    sync_channel.ul_prescaler = PWM_CMR_CPRE_CLKA,
    sync_channel.ul_period = SETTINGS_PWM_PERIOD,
    sync_channel.ul_duty = INIT_DUTY_VALUE,
    sync_channel.polarity = PWM_LOW,
    sync_channel.b_sync_ch = true,
    sync_channel.b_deadtime_generator = true,
    sync_channel.us_deadtime_pwmh = INIT_DEAD_TIME,
    sync_channel.us_deadtime_pwml = INIT_DEAD_TIME,
    sync_channel.output_selection.b_override_pwmh = false,//FIXME: implementar esto
    sync_channel.output_selection.b_override_pwml = false,
    sync_channel.output_selection.override_level_pwmh = PWM_LOW,
    sync_channel.output_selection.override_level_pwml = PWM_LOW,

    // TODO: adc

    // Initialize channels
    sync_channel.channel = SETTINGS_PWM_DRV1_CHANNEL; // channel 2
    pwm_channel_init(PWM, &sync_channel);

    sync_channel.channel = SETTINGS_PWM_DRV2_CHANNEL; //channel 0
    pwm_channel_init(PWM, &sync_channel);

    pwm_sync_init(PWM, PWM_SYNC_UPDATE_MODE_2, SYNC_UPDATE_PERIOD);

    /*
     * Request PDC transfer as soon as the synchronous update period elapses
     * (comparison unit is ignored).
     */
    pwm_pdc_set_request_mode(PWM, PWM_PDC_UPDATE_PERIOD_ELAPSED, (1 << 0));

    /* Configure interrupt for PDC transfer */
    pwm_pdc_enable_interrupt(PWM, PWM_PDC_TX_END);

    pwm_channel_enable(PWM, SETTINGS_PWM_DRV2_CHANNEL); //enable both by enabling channel 0
}

/** @brief PWM enter fault mode. */
void signal_pwm_enable(bool b_enable)
{
    // pwm_output_t channel_output = {
    //     .b_override_pwmh = b_enable,
    //     .b_override_pwml = b_enable
    // };
    // sync_channel.channel = SETTINGS_PWM_DRV1_CHANNEL;
    // pwm_channel_update_output(PWM, &sync_channel, &channel_output, true);

    // sync_channel.channel = SETTINGS_PWM_DRV2_CHANNEL;
    // pwm_channel_update_output(PWM, &sync_channel, &channel_output, true);

}

/** @}*/
