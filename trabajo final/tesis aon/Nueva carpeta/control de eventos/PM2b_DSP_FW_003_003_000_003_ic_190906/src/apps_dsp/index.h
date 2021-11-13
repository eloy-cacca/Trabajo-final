// $Header
/// @file			index.h
/// @brief  		Index generation file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		07 November 2016
/// @details
/// @copyright
///

#ifndef __INDEX_H__
#define __INDEX_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"
#include "signal_processing.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

// index_accum_t //////////////////////////////////////////////////////////////

typedef struct {

	int32_t real;
	int32_t imag;

} index_complex32_t;

typedef struct {

	// Accumulator of signal^2
	int64_t rms2;

	// Accumulator for harmonics
	index_complex32_t harmonic[SETTINGS_DSP_NDFT_MAX+1]; // harmonics + DC value

} index_accum_ch_t;

typedef struct {

	index_complex32_t pos;
	index_complex32_t neg;
	index_complex32_t hom;

} index_seq_t;

typedef struct {

	uint16_t accum;

	// sampling period
	int32_t Ts;

	// maximum time utilization
	int16_t time_utilization;

	// channel
	index_accum_ch_t ch[CH_ADC_GRID_TOTAL_NUM];

	// sequences
	index_seq_t v_seq;
	index_seq_t iA_seq;
	index_seq_t iB_seq;

	// Accumulator of v*i
	int64_t inst_power[CH_POWER_TOTAL_NUM];

} index_accum_t;

typedef struct {

	int16_t real;
	int16_t imag;

} index_complex16_t;

typedef struct {

	// Accumulator of signal^2
	int16_t rms;

	// Accumulator for harmonics
	index_complex16_t harmonic[SETTINGS_DSP_NDFT_MAX+1]; // harmonics + DC value

	// (Total waveform distortion)^2
	int16_t twd;

} index_result_ch_t;

typedef struct {

	int16_t ApparentPower;	// -Q14
	int16_t ActivePower;	// -Q14
	int16_t NoactivePower;	// -Q14
	int16_t PowerFactor;	// -Q14

} index_result_ch_power_t;

typedef struct {

	index_complex16_t pos;
	index_complex16_t neg;
	index_complex16_t hom;

} index_result_seq_t;

typedef struct {

	// flags
	uint16_t flags;

	// maximum time utilization
	int16_t time_utilization;

	// sampling period
	int16_t Ts;

	// frequency
	int16_t fL;

	// channel
	index_result_ch_t ch[CH_ADC_GRID_TOTAL_NUM];

	// sequences
	index_result_seq_t v_seq;
	index_result_seq_t iA_seq;
	index_result_seq_t iB_seq;

	// desbalance
	int16_t v_desb_neg;
	int16_t v_desb_hom;
	int16_t iA_desb_neg;
	int16_t iA_desb_hom;
	int16_t iB_desb_neg;
	int16_t iB_desb_hom;

	// Power
	index_result_ch_power_t ch_power[CH_POWER_TOTAL_NUM];

} index_result_t;


/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void index_update_accum( sp_t * data , index_accum_t * accum );
void index_update_result( index_accum_t * accum , index_result_t * result );

/// @endcond

#endif // __INDEX_H__

