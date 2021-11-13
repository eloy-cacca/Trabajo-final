// $Header
/// @file			signal_processing.h
/// @brief  		Signal processing file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		24 October 2016
/// @details
/// @copyright
///

#ifndef __SIGNAL_PROCESSING_H__
#define __SIGNAL_PROCESSING_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"

#include "../apps/unit_config.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

typedef struct {

	int64_t real;
	int64_t imag;

} sp_complex_t;

typedef struct {

	// Sliding window
	int32_t  buffer[SETTINGS_DSP_NPLL];
	uint16_t buffer_index;

	// Accumulator of signal^2
	int64_t rms2;

	// Accumulator for harmonics
	sp_complex_t harmonic[SETTINGS_DSP_NDFT_MAX+1]; // harmonics + DC value
	uint16_t     harmonic_index[SETTINGS_DSP_NDFT_MAX+1];

} sp_channel_t;

typedef struct {
	sp_complex_t pos;
	sp_complex_t neg;
	sp_complex_t hom;
} sp_seq_t;

typedef struct {

	// sampling period
	int32_t Ts;

	// channel
	sp_channel_t ch[CH_ADC_GRID_TOTAL_NUM];

	// sequences
	sp_seq_t v_seq;
	sp_seq_t iA_seq;
	sp_seq_t iB_seq;

	// Accumulator of v*i
	int64_t inst_power[CH_POWER_TOTAL_NUM];

} sp_t;



/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void processing_update( int32_t * signal , sp_t * data );

/// @endcond

#endif // __SIGNAL_PROCESSING_H__

