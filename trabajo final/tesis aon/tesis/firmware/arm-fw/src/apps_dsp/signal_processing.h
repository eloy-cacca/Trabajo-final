/** $Header
 * @file            signal_processing.h
 * @brief           Signal processing module.
 * @details
 * @author          Agustin Aon
 * @version         0.1
 * @date            18 September 2020
 * @details
 * @copyright
 */

#ifndef SIGNAL_PROCESSING_H_
#define SIGNAL_PROCESSING_H_


/**************************** INCLUDES ***************************************/
#include "settings.h"


/**************************** ENUMS, STRUCTS *********************************/
typedef struct {
    int64_t  real;                                        // Q30
    int64_t  imag;                                        // Q30
} sp_complex_t;

typedef struct {
    int32_t       buffer[SETTINGS_MSDFT_BUF_LEN];         // Sliding window buffer
    uint16_t      buffer_index;                           // Sliding window index
    int64_t       rms2;                                   // Q24
    sp_complex_t  harmonic[SETTINGS_NDFT_MAX + 1];        // harmonics + DC value
    uint16_t      harmonic_index[SETTINGS_NDFT_MAX + 1];
} sp_channel_t;


typedef struct {
    sp_channel_t ch[SETTINGS_CH_TOTAL_NUM];
} sp_t;

typedef struct {
    uint8_t   enabled;
    uint16_t  amp;
    uint16_t  fL;
    uint8_t   custom_waveform_b;
}  signal_params_t;

/**************************** FUNCTIONS **************************************/
void signal_process(int32_t *signals, sp_t *sp);


#endif /* SIGNAL_PROCESSING_H_ */