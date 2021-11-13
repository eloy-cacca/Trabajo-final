/**
 * $Header
 * @file           index.c
 * @brief          index module.
 * @details
 * @author         Agustin Aon
 * @version        0.01
 * @date           16 September 2020
 * @details
 * @copyright
 *
 * @addtogroup main
 * @{
 */


/**************************** INCLUDES ***************************************/
#include "index.h"
#include <math.h>
#include "utils/byte_string_operations.h"
#include "utils/calculus.h"
#include "apps/control.h"


/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief Update index accumulators. */
void index_update_accum(sp_t *data, index_accum_t *accum)
{
    // accum
    accum->accum++;

    // flags
    if (accum->accum != 1)
    {
        control_check_param_change(&accum->flags, &accum->params);
    }

    // signal parameters
    signal_params_t *params = control_get_params();
    copy_content((uint8_t *)params, (uint8_t *)&accum->params, sizeof(signal_params_t));

    // rms2 and harmonics
    uint8_t ch_num;
    uint8_t h_num;

    for (ch_num = 0; ch_num < SETTINGS_CH_TOTAL_NUM; ch_num++)
    {
        accum->ch[ch_num].rms2 += data->ch[ch_num].rms2;

        for (h_num = 0; h_num < (SETTINGS_NDFT_MAX + 1); h_num++)
        {
            accum->ch[ch_num].harmonic[h_num].real += (data->ch[ch_num].harmonic[h_num].real >> 18);
            accum->ch[ch_num].harmonic[h_num].imag += (data->ch[ch_num].harmonic[h_num].imag >> 18);
        }
    }
}

/** @brief Update index result from 10 cycles. */
void index_update_result(index_accum_t *accum, index_result_t *result)
{
    // flags
    result->flags = accum->flags;

    // signal parameters
    copy_content((uint8_t *)&accum->params, (uint8_t *)&result->params, sizeof(signal_params_t));

    // rms and harmonics
    uint8_t ch_num;
    uint8_t h_num;

    for (ch_num = 0; ch_num < SETTINGS_CH_TOTAL_NUM; ch_num++)
    {
        // rms
        int64_t rms2 = (accum->ch[ch_num].rms2 >> 7) / accum->accum; // >>7 because of sliding window
        result->ch[ch_num].rms = (int32_t) sqrt(rms2);

        // harmonics
        for (h_num = 0; h_num < (SETTINGS_NDFT_MAX + 1); h_num++)
        {
            result->ch[ch_num].harmonic[h_num].real = accum->ch[ch_num].harmonic[h_num].real / accum->accum;
            result->ch[ch_num].harmonic[h_num].imag = accum->ch[ch_num].harmonic[h_num].imag / accum->accum;
        }

        // TWD^2 = (rms^2 - H1rms^2) / H1rms^2
        int64_t H1rms2 = (SQUARE(result->ch[ch_num].harmonic[1].real) + SQUARE(result->ch[ch_num].harmonic[1].imag)) / 2;  // Q24, divided by 2 to get RMS
        int64_t twd2_temp = rms2 - H1rms2;

        if (twd2_temp < 0)
        {
            result->ch[ch_num].twd = 0;
        }
        else
        {
            result->ch[ch_num].twd = sqrt((twd2_temp << 24) / H1rms2);  // Q12
        }
    }

    // restart accum
    memset((void *)accum, 0, sizeof(*accum));
}
