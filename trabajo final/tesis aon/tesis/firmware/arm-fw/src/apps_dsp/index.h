/** $Header
 * @file            index.h
 * @brief           index module.
 * @details
 * @author          Agustin Aon
 * @version         0.1
 * @date            16 September 2020
 * @details
 * @copyright
 */

#ifndef INDEX_H_
#define INDEX_H_


/**************************** INCLUDES ***************************************/
#include "settings.h"
#include "signal_processing.h"


/**************************** ENUMS, STRUCTS *********************************/
typedef struct {
    int64_t  real;                                       // Q12
    int64_t  imag;                                       // Q12
} index_complex64_t;

typedef struct {
    int64_t            rms2;                             // Q24
    index_complex64_t  harmonic[SETTINGS_NDFT_MAX + 1];
} index_accum_ch_t;

typedef struct {
    uint8_t           flags;                             // flags
    uint16_t          accum;                             // counter of accumulated data
    signal_params_t   params;                            // current signal parameters
    index_accum_ch_t  ch[SETTINGS_CH_TOTAL_NUM];         // channels data
} index_accum_t;

typedef struct {
    int32_t  real;                                       // Q12
    int32_t  imag;                                       // Q12
} index_complex32_t;

typedef struct {
    int32_t            rms;                              // Q12
    int16_t            twd;                              // Q12
    index_complex32_t  harmonic[SETTINGS_NDFT_MAX + 1];
} index_result_ch_t;

typedef struct {
    uint8_t            flags;                            // flags
    signal_params_t    params;                           // current signal parameters
    index_result_ch_t  ch[SETTINGS_CH_TOTAL_NUM];        // channels data
} index_result_t;


/**************************** FUNCTIONS **************************************/
void index_update_accum(sp_t *data, index_accum_t *accum);
void index_update_result(index_accum_t *accum, index_result_t *result);


#endif /* INDEX_H_ */