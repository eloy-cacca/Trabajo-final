/**
 * $Header
 * @file           trend.c
 * @brief          trend module.
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
#include "trend.h"
#include "utils/calculus.h"
#include "apps/control.h"
#include "utils/byte_string_operations.h"
#include <math.h>


/**************************** MACROS *****************************************/
#define INT32_MIN_VALUE ((int32_t)0x80000000)         // minimum initial value
#define INT32_MAX_VALUE ((int32_t)0x7FFFFFFF)         // maximum initial value
#define INT64_MIN_VALUE ((int64_t)0x8000000000000000) // minimum initial value
#define INT64_MAX_VALUE ((int64_t)0x7FFFFFFFFFFFFFFF) // maximum initial value


/**************************** LOCAL FUNCTIONS ********************************/
static void set_initial_int32(int32_t *out)
{
    out[0] = 0;
    out[1] = INT32_MIN_VALUE;
    out[2] = INT32_MAX_VALUE;
}

static void set_initial_int64(int64_t *out)
{
    out[0] = 0;
    out[1] = INT64_MIN_VALUE;
    out[2] = INT64_MAX_VALUE;
}

static void update_AvgMaxMin_int16_int32(int16_t *in, int32_t *out)
{
    out[trend_avg] += *in;
    out[trend_max] = MAX(out[trend_max], *in);
    out[trend_min] = MIN(out[trend_min], *in);
}

static void update_AvgMaxMin_int32_int64(int32_t *in, int64_t *out)
{
    out[trend_avg] += *in;
    out[trend_max] = MAX(out[trend_max], *in);
    out[trend_min] = MIN(out[trend_min], *in);
}

static void set_AvgMaxMin_int64_int32(int64_t *in, int32_t *out, uint16_t accum_N)
{
    out[trend_avg] = in[trend_avg] / accum_N;
    out[trend_max] = in[trend_max];
    out[trend_min] = in[trend_min];
}

static void set_AvgMaxMin_int32_int32(int32_t *in, int32_t *out, uint16_t accum_N)
{
    out[trend_avg] = in[trend_avg] / accum_N;
    out[trend_max] = in[trend_max];
    out[trend_min] = in[trend_min];
}

static void set_AvgMaxMin_sqrt_int64_int32(int64_t *in, int32_t *out, uint16_t accum_N)
{
    out[trend_avg] = sqrt(in[trend_avg] / accum_N);
    out[trend_max] = sqrt(in[trend_max]);
    out[trend_min] = sqrt(in[trend_min]);
}

static void set_AvgMaxMin_sqrt_int64_int16(int64_t *in, int16_t *out, uint16_t accum_N)
{
    out[trend_avg] = sqrt(in[trend_avg] / accum_N);
    out[trend_max] = sqrt(in[trend_max]);
    out[trend_min] = sqrt(in[trend_min]);
}

/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief Reset accumulator */
void trend_reset_accum(trend_accum_t *accum)
{
    // accum, flags, params
    accum->accum = 0;
    accum->flags = 0;
    accum->params.amp = 0;
    accum->params.fL = 0;

    // channels
    uint16_t ch_num;
    uint16_t h_num;

    for (ch_num = 0; ch_num < SETTINGS_CH_TOTAL_NUM; ch_num++)
    {
        // RMS
        // NOTE: RMS accumulator is stored in int64_t so set_min can't be used
        set_initial_int64(accum->ch[ch_num].rms2);

        // Harmonics
        for (h_num = 0; h_num < (SETTINGS_NDFT_MAX + 1); h_num++)
        {
            set_initial_int64(accum->ch[ch_num].harmonic[h_num].real);
            set_initial_int64(accum->ch[ch_num].harmonic[h_num].imag);
        }

        // TWD
        set_initial_int64(accum->ch[ch_num].twd2);
    }
}

/** @brief reset result */
void trend_reset_result(trend_result_t *trend)
{
    memset((void *)trend, 0, sizeof(trend_result_t));
}

/** @brief Update trend accumulators. */
void trend_update_accum(index_result_t *data, trend_accum_t *accum)
{
    // increase accumulator
    accum->accum++;

    // flags
    if (accum->accum != 1)
    {
        control_check_param_change(&accum->flags, &accum->params);
    }
    SET_FLAG(accum->flags, data->flags);

    // signal parameters
    copy_content((uint8_t *)&data->params, (uint8_t *)&accum->params, sizeof(signal_params_t));

    // update RMS and harmonics accumulator
    uint16_t ch_num;
    uint16_t h_num;

    for (ch_num = 0; ch_num < SETTINGS_CH_TOTAL_NUM; ch_num++)
    {
        int32_t temp_square;

        // RMS
        temp_square = SQUARE((int32_t)data->ch[ch_num].rms);
        update_AvgMaxMin_int32_int64(&temp_square, &accum->ch[ch_num].rms2[0]);

        // Harmonics
        for (h_num = 0; h_num < (SETTINGS_NDFT_MAX + 1); h_num++)
        {
            update_AvgMaxMin_int32_int64(&data->ch[ch_num].harmonic[h_num].real, &accum->ch[ch_num].harmonic[h_num].real[0]);
            update_AvgMaxMin_int32_int64(&data->ch[ch_num].harmonic[h_num].imag, &accum->ch[ch_num].harmonic[h_num].imag[0]);
        }

        // Total Waveform Distortion (TWD)
        temp_square = SQUARE((int32_t)data->ch[ch_num].twd);
        update_AvgMaxMin_int32_int64(&temp_square, &accum->ch[ch_num].twd2[0]);
    }
}

/** @brief Update trend result. */
void trend_update_result(trend_accum_t *accum, trend_result_t *result)
{
    // update flags
    result->flags = accum->flags;

    // signal parameters
    copy_content((uint8_t *)&accum->params, (uint8_t *)&result->params, sizeof(signal_params_t));

    // update RMS and harmonics accumulator
    {
        uint16_t ch_num;
        uint16_t h_num;

        for (ch_num = 0; ch_num < SETTINGS_CH_TOTAL_NUM; ch_num++)
        {
            // RMS
            set_AvgMaxMin_sqrt_int64_int32(&accum->ch[ch_num].rms2[0], &result->ch[ch_num].rms[0], accum->accum);

            // Harmonics
            for (h_num = 0; h_num < (SETTINGS_NDFT_MAX + 1); h_num++)
            {
                set_AvgMaxMin_int64_int32(&accum->ch[ch_num].harmonic[h_num].real[0], &result->ch[ch_num].harmonic[h_num].real[0], accum->accum);
                set_AvgMaxMin_int64_int32(&accum->ch[ch_num].harmonic[h_num].imag[0], &result->ch[ch_num].harmonic[h_num].imag[0], accum->accum);
            }

            // Total Waveform Distortion (TWD)
            set_AvgMaxMin_sqrt_int64_int16(&accum->ch[ch_num].twd2[0], &result->ch[ch_num].twd[0], accum->accum);
        }
    }
}
