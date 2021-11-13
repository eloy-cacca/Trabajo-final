// $Header
/// @file			trend.h
/// @brief  		Trend generation file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		24 November 2016
/// @details
/// @copyright
///

#ifndef __TREND_H__
#define __TREND_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"
#include "index.h"
#include "../apps/rtc.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

enum trend_enum{
	trend_avg = 0,
	trend_max,
	trend_min,
	trend_TOTAL_NUM
	};


typedef struct {

	int32_t real[3];
	int32_t imag[3];

} trend_complex32_t;

typedef struct {

	// Accumulator of signal^2
	int64_t rms2[3];

	// Accumulator for harmonics
	trend_complex32_t harmonic[SETTINGS_DSP_NDFT_MAX+1]; // harmonics + DC value

	// (Total waveform distortion)^2
	int64_t twd2[3];

} trend_accum_ch_t;

typedef struct {

	int32_t ApparentPower[3]; // -Q14
	int32_t ActivePower[3];   // -Q14
	int32_t NoactivePower[3]; // -Q14
	int32_t PowerFactor[3];   // -Q14

} trend_accum_ch_power_t;

typedef struct {

	trend_complex32_t pos;
	trend_complex32_t neg;
	trend_complex32_t hom;

} trend_acumm_seq_t;

typedef struct {

	uint16_t accum; // used only in accumulator process

	// flags
	uint16_t flags;

	// maximum time utilization
	int16_t time_utilization;

	// sampling period
	int32_t Ts[3];

	// frequency
	int32_t fL[3];

	// channel
	trend_accum_ch_t ch[CH_ADC_GRID_TOTAL_NUM];

	// sequences
	trend_acumm_seq_t v_seq;
	trend_acumm_seq_t iA_seq;
	trend_acumm_seq_t iB_seq;

	// desbalance
	int32_t v_desb_neg[3];
	int32_t v_desb_hom[3];
	int32_t iA_desb_neg[3];
	int32_t iA_desb_hom[3];
	int32_t iB_desb_neg[3];
	int32_t iB_desb_hom[3];

	// Power
	trend_accum_ch_power_t ch_power[CH_POWER_TOTAL_NUM];

} trend_accum_t;

//***************************************************************************//

// trend_t is defined in trend_common.h


/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void trend_reset_accum( trend_accum_t * accum );
void trend_reset_result( trend_t * trend );

void trend_update_accum_from_index( index_result_t * data , trend_accum_t * accum );
void trend_update_accum( trend_t * data , trend_accum_t * accum );
void trend_update_result( trend_accum_t * accum , trend_t * result );

/// @endcond

#endif // __TREND_H__

