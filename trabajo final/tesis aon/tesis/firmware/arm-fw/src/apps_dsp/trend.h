/** $Header
  * @file           trend.h
  * @brief          trend module.
  * @details
  * @author         Agustin Aon
  * @version        0.1
  * @date           16 September 2020
  * @details
  * @copyright
  */

#ifndef TREND_H_
#define TREND_H_


/**************************** INCLUDES ***************************************/
#include "settings.h"
#include "index.h"
#include "signal_processing.h"


/**************************** ENUMS, STRUCTS *********************************/
enum trend_enum{
    trend_avg = 0,
    trend_max,
    trend_min,
    trend_TOTAL_NUM
};

typedef struct {
    int64_t  real[3];                                    // Q12
    int64_t  imag[3];                                    // Q12
} trend_complex64_t;

typedef struct {
    int64_t            rms2[3];                          // Q24
    int64_t            twd2[3];                          // Q12
    trend_complex64_t  harmonic[SETTINGS_NDFT_MAX + 1];  // harmonics + DC value
} trend_accum_ch_t;

typedef struct {
    uint8_t           flags;
    signal_params_t   params;
    uint16_t          accum;
    trend_accum_ch_t  ch[SETTINGS_CH_TOTAL_NUM];
} trend_accum_t;

typedef struct {
    int32_t  real[3];                                    // Q12
    int32_t  imag[3];                                    // Q12
} trend_complex32_t;

typedef struct {
    int32_t            rms[3];                           // Q12
    int16_t            twd[3];                           // Q12
    trend_complex32_t  harmonic[SETTINGS_NDFT_MAX + 1];  // harmonics + DC value
} trend_result_ch_t;

typedef struct {
    uint8_t            flags;
    signal_params_t    params;
    trend_result_ch_t  ch[SETTINGS_CH_TOTAL_NUM];
} trend_result_t;


/**************************** FUNCTIONS **************************************/
void trend_update_accum(index_result_t *data, trend_accum_t *accum);
void trend_update_result(trend_accum_t *accum, trend_result_t *result);
void trend_reset_accum(trend_accum_t *accum);
void trend_reset_result(trend_result_t *trend);


#endif /* TREND_H_ */