/**
 * $Header
 * @file           signal_processing.c
 * @brief          Signal processing module.
 * @details
 * @author         Agustin Aon
 * @version        0.1
 * @date           18 September 2020
 * @details
 * @copyright
 *
 * @addtogroup main
 * @{
 */

/**************************** INCLUDES ***************************************/
#include "signal_processing.h"
#include "apps/control.h"
#include "utils/iqmath.h"
#include "utils/lut.h"


/**************************** LOCAL FUNCTIONS ********************************/
static void msdft_modulation(uint16_t H, uint16_t *index, int32_t *input, sp_complex_t *value)
{
    // mSDFT Index
    *index += H;
    if (*index > (SETTINGS_MSDFT_BUF_LEN - 1))
        *index -= SETTINGS_MSDFT_BUF_LEN;

    // Modulator and Resonator
    // NOTE: Output is in Q(7 - 1 + 12) = Q18
    //  Gain of SW is Q7
    //  Gain of modulation 1/2
    //  Sine and cosine function are in Q12 (Q20>>8=Q12)
    value->real += (*input) * (lut_cos_128_Q20(*index) >> 8);
    value->imag -= (*input) * (lut_sin_128_Q20(*index) >> 8);
}


/**************************** GLOBAL FUNCTIONS *******************************/

/* @brief Process incoming signal */
void signal_process(int32_t *signals, sp_t *sp)
{
    uint8_t ch_num;

    // Update channel parameters
    for (ch_num = 0; ch_num < SETTINGS_CH_TOTAL_NUM; ch_num++)
    {
        int32_t      SW_out;
        int32_t      *input         = &signals[ch_num];
        sp_channel_t *ch            = &sp->ch[ch_num];
        int32_t      *signal_last_N = &ch->buffer[ch->buffer_index];

        // Update accumulator of signal^2 (used for RMS calculation)
        ch->rms2 += FPMUL(*input, *input, 0);
        ch->rms2 -= FPMUL(*signal_last_N, *signal_last_N, 0);

        // Update Sliding window output
        SW_out = *input - *signal_last_N;

        // Update sliding window buffer and index
        ch->buffer[ch->buffer_index] = *input;
        ch->buffer_index++;
        if (ch->buffer_index > (SETTINGS_MSDFT_BUF_LEN - 1))
            ch->buffer_index = 0;

        // Update DC accumulator (MAF). Harmonics are stored in Q18 so data is shifted by 11. With SW length (<<7) it results in Q18
        ch->harmonic[0].real += (SW_out << 11);
        ch->harmonic[0].imag = 0;

        // Update harmonics accumulators (Modulated Sliding DFT)
        signal_params_t *signal_params = control_get_params();

        for (uint8_t i = 1; i < 5; i++)
        {
            uint16_t index_increment = signal_params->fL / 50 * (2*i - 1);

            if (index_increment < (SETTINGS_MSDFT_BUF_LEN / 2))  // Max harmonic
            {
                msdft_modulation(index_increment, &ch->harmonic_index[i], &SW_out, &ch->harmonic[i]);
            }
        }
    }
}
