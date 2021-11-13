/**
 * $Header
 * @file           signal_adc.c
 * @brief          adc function.
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
#include "signal_adc.h"


/**************************** MACROS *****************************************/

/* Tracking Time */
#define TRACKING_TIME 1
/* Transfer Period */
#define TRANSFER_PERIOD 1

/** ADC convention done mask. */
#define ADC_DONE_MASK ((1 << SETTINGS_CH_TOTAL_NUM) - 1)

/** Size of the receive buffer and transmit buffer. */
#define BUFFER_SIZE SETTINGS_CH_TOTAL_NUM

/** The maximal digital value */
#define MAX_DIGITAL (4095)

/** Reference voltage for ADC, in V. */
#define VOLT_REF (3)


/**************************** ENUMS, STRUCTS *********************************/

/** ADC sample data */
typedef struct
{
    uint8_t uc_ch_num[SETTINGS_CH_TOTAL_NUM];
    uint16_t us_value[SETTINGS_CH_TOTAL_NUM];
    uint16_t us_done;
} sample_data_t;


/**************************** LOCAL VARIABLES ********************************/
static sample_data_t sample_data;

static enum adc_channel_num_t ch_list[SETTINGS_CH_TOTAL_NUM] =
    {
        ADC_CHANNEL_3, // I1: shunt
        ADC_CHANNEL_9, // I2
        ADC_CHANNEL_2, // I3
        ADC_CHANNEL_8  // I4
};


/**************************** LOCAL FUNCTIONS ********************************/

/**
 * @brief Read converted data through PDC channel.
 *
 * @param p_adc The pointer of adc peripheral.
 * @param p_s_buffer The destination buffer.
 * @param ul_size The size of the buffer.
 */
static uint32_t signal_adc_read_buffer(Adc *p_adc, uint16_t *p_s_buffer, uint32_t ul_size)
{
    /* Check if the first PDC bank is free. */
    if ((p_adc->ADC_RCR == 0) && (p_adc->ADC_RNCR == 0))
    {
        p_adc->ADC_RPR = (uint32_t)p_s_buffer;
        p_adc->ADC_RCR = ul_size;
        p_adc->ADC_PTCR = ADC_PTCR_RXTEN;

        return 1;
    }
    else /* Check if the second PDC bank is free. */
    {
        if (p_adc->ADC_RNCR == 0)
        {
            p_adc->ADC_RNPR = (uint32_t)p_s_buffer;
            p_adc->ADC_RNCR = ul_size;

            return 1;
        }
        else
            return 0;
    }
}


/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief Get sampled signals from ADC */
void signal_adc_update(int32_t *signals)
{
#if 1 // take data from LUT to test
#include "apps/control.h"
#include <math.h>
#include "utils/lut.h"
#include "utils/iqmath.h"

#define SQRT2_Q15 (46341)

    static uint16_t phase_1 = 0;
    static uint16_t phase_3 = 0;
    signal_params_t *sp = control_get_params();

    // output
    uint8_t i;
    for (i = 0; i < SETTINGS_CH_TOTAL_NUM; i++)
    {
        signals[i] = FPMUL(lut_sin_128_Q20(phase_1), sp->amp, 8) + FPMUL(lut_sin_128_Q20(phase_3), sp->amp / 10, 8);
    }

    // increase phases
    phase_1 += sp->fL / 50;
    phase_3 += sp->fL / 50 * 3;

    if (phase_1 >= SETTINGS_MSDFT_BUF_LEN)
        phase_1 -= SETTINGS_MSDFT_BUF_LEN;
    if (phase_3 >= SETTINGS_MSDFT_BUF_LEN)
        phase_3 -= SETTINGS_MSDFT_BUF_LEN;

#else
    uint8_t i;
    for (i = 0; i < SETTINGS_CH_TOTAL_NUM; i++)
    {
        signals[i] = (int32_t)((((uint64_t)(sample_data.us_value[i] * VOLT_REF) << 15) / MAX_DIGITAL)); // TODO: calibracion y eso
    }
#endif
}

/** @brief Initialize ADC. */
void signal_adc_init()
{

    // Initialize sample variable
    memset((void *)&sample_data, 0, sizeof(sample_data));

    /* Enable peripheral clock. */
    uint32_t i;
    pmc_enable_periph_clk(ID_ADC);

    /* Initialize ADC.
     *
     * Formula 1: ADCClock = MCK / ( (PRESCAL+1) * 2 )
     * For example, MCK = 64MHZ, PRESCAL = 4, then:
     * ADCClock = 64 / ((4+1) * 2) = 6.4MHz;
     *
     * Formula 2:
     *     Startup  Time = startup value / ADCClock
     *     Startup time = 64 / 6.4MHz = 10 us
     */
    adc_init(ADC, sysclk_get_cpu_hz(), 6400000, ADC_STARTUP_TIME_4);

    /* Set ADC timing.
     *
     * Formula:
     *     Transfer Time = (TRANSFER * 2 + 3) / ADCClock
     *     Tracking Time = (TRACKTIM + 1) / ADCClock
     *     Settling Time = settling value / ADCClock
     *
     *     Transfer Time = (1 * 2 + 3) / 6.4MHz = 781 ns
     *     Tracking Time = (1 + 1) / 6.4MHz = 312 ns
     *     Settling Time = 3 / 6.4MHz = 469 ns
     */
    adc_configure_timing(ADC, TRACKING_TIME, ADC_SETTLING_TIME_3, TRANSFER_PERIOD);

    /* Enable channel number tag. */
    adc_enable_tag(ADC);

    /* Enable sequencer. */
    adc_configure_sequence(ADC, ch_list, SETTINGS_CH_TOTAL_NUM);
    adc_start_sequencer(ADC);

    /* Enable channels. */
    for (i = 0; i < SETTINGS_CH_TOTAL_NUM; i++)
    {
        adc_enable_channel(ADC, (enum adc_channel_num_t)i);
    }

    /* Update channel number. */
    sample_data.uc_ch_num[0] = ch_list[0];

    /* Set gain and offset (only single ended mode used here). */
    adc_disable_anch(ADC); /* Disable analog change. */
    for (i = 0; i < SETTINGS_CH_TOTAL_NUM; i++)
    {
        adc_set_channel_input_gain(ADC, ch_list[i], ADC_GAINVALUE_0);
        adc_disable_channel_input_offset(ADC, ch_list[i]);
    }

    /* Auto calibrate. */
    adc_set_calibmode(ADC);
    while (1)
    {
        if ((adc_get_status(ADC) & ADC_ISR_EOCAL) == ADC_ISR_EOCAL)
            break;
    }

    /* Set power save. */
    adc_configure_power_save(ADC, 0, 0);

    /* Transfer with PDC. */
    signal_adc_read_buffer(ADC, sample_data.us_value, BUFFER_SIZE);
    adc_enable_interrupt(ADC, ADC_IER_RXBUFF);

    /* Configure trigger mode and start convention. */
    adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
}

/** @brief ADC software trigger. */
void signal_adc_start_conversion()
{
    adc_start(ADC);
}

/** @brief ADC callback function. */
void signal_adc_process_data()
{
    uint32_t i;

    if ((adc_get_status(ADC) & ADC_ISR_RXBUFF) == ADC_ISR_RXBUFF)
    {
        sample_data.us_done = ADC_DONE_MASK;
        signal_adc_read_buffer(ADC, sample_data.us_value, BUFFER_SIZE);
        /* Only keep sample value, and discard channel number. */
        for (i = 0; i < SETTINGS_CH_TOTAL_NUM; i++)
        {
            sample_data.us_value[i] &= ADC_LCDR_LDATA_Msk;
        }
    }
}
