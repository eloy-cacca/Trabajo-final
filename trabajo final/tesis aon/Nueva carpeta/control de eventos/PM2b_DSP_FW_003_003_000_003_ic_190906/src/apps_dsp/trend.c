// $Header
/// @file			trend.c
/// @brief  		Trend generation file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		24 November 2016
/// @details
/// @copyright
///

/// @addtogroup apps
/// @{

/**************************** INCLUDES ***************************************/

#include "trend.h"
#include "../utils/iqmath.h"
#include "../utils/calculus.h"
#include "../apps/rtc.h"
#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

#define INT32_MIN_VALUE   ((int32_t)0x80000000) 		// minimum initial value
#define INT32_MAX_VALUE   ((int32_t)0x7FFFFFFF) 		// maximum initial value
#define INT64_MIN_VALUE   ((int64_t)0x8000000000000000) // minimum initial value
#define INT64_MAX_VALUE   ((int64_t)0x7FFFFFFFFFFFFFFF) // maximum initial value

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

void set_initial_int32( int32_t * out ) {
	out[0] = 0;
	out[1] = INT32_MIN_VALUE;
	out[2] = INT32_MAX_VALUE;
}

void set_initial_int64( int64_t * out ) {
	out[0] = 0;
	out[1] = INT64_MIN_VALUE;
	out[2] = INT64_MAX_VALUE;
}

void update_AvgMaxMin_1_int32( int16_t * in , int32_t * out ) {

	out[trend_avg] += *in;
	out[trend_max] = MAX( out[trend_max] , *in );
	out[trend_min] = MIN( out[trend_min] , *in );

}

void update_AvgMaxMin_1_int64( int32_t * in , int64_t * out ) {

	out[trend_avg] += *in;
	out[trend_max] = MAX( out[trend_max] , *in );
	out[trend_min] = MIN( out[trend_min] , *in );

}

void update_AvgMaxMin_2_int32( int16_t * in , int32_t * out ) {

	out[trend_avg] += in[trend_avg];
	out[trend_max] = MAX( out[trend_max] , in[trend_max] );
	out[trend_min] = MIN( out[trend_min] , in[trend_min] );

}

void update_AvgMaxMin_2_int64( int32_t * in , int64_t * out ) {

	out[trend_avg] += in[trend_avg];
	out[trend_max] = MAX( out[trend_max] , in[trend_max] );
	out[trend_min] = MIN( out[trend_min] , in[trend_min] );

}

void set_AvgMaxMin_int32( int32_t * in , int16_t * out , uint16_t accum_N  ) {

	out[trend_avg] = in[trend_avg] / accum_N;
	out[trend_max] = in[trend_max];
	out[trend_min] = in[trend_min];

}

void set_AvgMaxMin_int64( int64_t * in , int16_t * out , uint16_t accum_N  ) {

	out[trend_avg] = sqrt( in[trend_avg] / accum_N );
	out[trend_max] = sqrt( in[trend_max] );
	out[trend_min] = sqrt( in[trend_min] );

}

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief reset accumulator
void trend_reset_accum( trend_accum_t * accum ) {

	// increase accumulator
	accum->accum = 0;

	// update flags
	accum->flags = 0;

	// maximum time utilization
	accum->time_utilization = 0;

	// reset sampling period
	set_initial_int32( &accum->Ts[0] );

	// frequency
	set_initial_int32( &accum->fL[0] );

	{
		uint16_t CH_count;
		uint16_t H_count;

		for ( CH_count=0 ; CH_count<CH_ADC_GRID_TOTAL_NUM ; CH_count++ ) {

			// RMS
			// NOTE: RMS accumulator is stored in int64_t so set_min can't be used
			set_initial_int64( &accum->ch[CH_count].rms2[0] );

			// Harmonics
			for ( H_count=0 ; H_count<(SETTINGS_DSP_NDFT_MAX+1) ; H_count++ ) {

				set_initial_int32( &accum->ch[CH_count].harmonic[H_count].real[0] );
				set_initial_int32( &accum->ch[CH_count].harmonic[H_count].imag[0] );

			}

			// Total Waveform Distortion (TWD)
			set_initial_int64( &accum->ch[CH_count].twd2[0] );
		}

	}

	// Update voltage
	// positive sequence
	set_initial_int32( &accum->v_seq.pos.real[0] );
	set_initial_int32( &accum->v_seq.pos.imag[0] );
	// negative sequence
	set_initial_int32( &accum->v_seq.neg.real[0] );
	set_initial_int32( &accum->v_seq.neg.imag[0] );
	// homopolar sequence
	set_initial_int32( &accum->v_seq.hom.real[0] );
	set_initial_int32( &accum->v_seq.hom.imag[0] );

	// Update current A
	// positive sequence
	set_initial_int32( &accum->iA_seq.pos.real[0] );
	set_initial_int32( &accum->iA_seq.pos.imag[0] );
	// negative sequence
	set_initial_int32( &accum->iA_seq.neg.real[0] );
	set_initial_int32( &accum->iA_seq.neg.imag[0] );
	// homopolar sequence
	set_initial_int32( &accum->iA_seq.hom.real[0] );
	set_initial_int32( &accum->iA_seq.hom.imag[0] );

	// Update current B
	// positive sequence
	set_initial_int32( &accum->iB_seq.pos.real[0] );
	set_initial_int32( &accum->iB_seq.pos.imag[0] );
	// negative sequence
	set_initial_int32( &accum->iB_seq.neg.real[0] );
	set_initial_int32( &accum->iB_seq.neg.imag[0] );
	// homopolar sequence
	set_initial_int32( &accum->iB_seq.hom.real[0] );
	set_initial_int32( &accum->iB_seq.hom.imag[0] );

	// compute desbalance
	set_initial_int32( &accum->v_desb_neg[0] );
	set_initial_int32( &accum->v_desb_hom[0] );
	set_initial_int32( &accum->iA_desb_neg[0] );
	set_initial_int32( &accum->iA_desb_hom[0] );
	set_initial_int32( &accum->iB_desb_neg[0] );
	set_initial_int32( &accum->iB_desb_hom[0] );

	// Update power
	{
		uint16_t CH_count;

		for ( CH_count=0 ; CH_count<CH_POWER_TOTAL_NUM ; CH_count++ ) {

			set_initial_int32( &accum->ch_power[CH_count].ApparentPower[0] );
			set_initial_int32( &accum->ch_power[CH_count].ActivePower[0]   );
			set_initial_int32( &accum->ch_power[CH_count].NoactivePower[0] );
			set_initial_int32( &accum->ch_power[CH_count].PowerFactor[0]   );

		}

	}

}

/// @brief reset result
void trend_reset_result( trend_t * trend ) {

	memset( (void *)trend  , 0 , sizeof(trend_t) );

}

/// @brief Update accumulator with index data
void trend_update_accum_from_index( index_result_t * data , trend_accum_t * accum ) {

	// increase accumulator
	accum->accum++;

	// update flags
	accum->flags |= data->flags;

	// maximum time utilization
	accum->time_utilization = MAX( data->time_utilization , accum->time_utilization );

	// update sampling period
	update_AvgMaxMin_1_int32( &data->Ts , &accum->Ts[0] );

	// frequency
	update_AvgMaxMin_1_int32( &data->fL , &accum->fL[0] );

	// update RMS and harmonics accumulator
	{
		uint16_t CH_count;
		uint16_t H_count;

		for ( CH_count=0 ; CH_count<CH_ADC_GRID_TOTAL_NUM ; CH_count++ ) {

			int32_t temp_square;

			// RMS
			temp_square = SQUARE( (int32_t)data->ch[CH_count].rms );
			update_AvgMaxMin_1_int64( &temp_square , &accum->ch[CH_count].rms2[0] );

			// Harmonics
			for ( H_count=0 ; H_count<(SETTINGS_DSP_NDFT_MAX+1) ; H_count++ ) {

				update_AvgMaxMin_1_int32( &data->ch[CH_count].harmonic[H_count].real , &accum->ch[CH_count].harmonic[H_count].real[0] );
				update_AvgMaxMin_1_int32( &data->ch[CH_count].harmonic[H_count].imag , &accum->ch[CH_count].harmonic[H_count].imag[0] );

			}

			// Total Waveform Distortion (TWD)
			temp_square = SQUARE( (int32_t)data->ch[CH_count].twd );
			update_AvgMaxMin_1_int64( &temp_square , &accum->ch[CH_count].twd2[0] );
		}

	}

	// Update voltage
	// positive sequence
	update_AvgMaxMin_1_int32( &data->v_seq.pos.real , &accum->v_seq.pos.real[0] );
	update_AvgMaxMin_1_int32( &data->v_seq.pos.imag , &accum->v_seq.pos.imag[0] );
	// negative sequence
	update_AvgMaxMin_1_int32( &data->v_seq.neg.real , &accum->v_seq.neg.real[0] );
	update_AvgMaxMin_1_int32( &data->v_seq.neg.imag , &accum->v_seq.neg.imag[0] );
	// homopolar sequence
	update_AvgMaxMin_1_int32( &data->v_seq.hom.real , &accum->v_seq.hom.real[0] );
	update_AvgMaxMin_1_int32( &data->v_seq.hom.imag , &accum->v_seq.hom.imag[0] );

	// Update current A
	// positive sequence
	update_AvgMaxMin_1_int32( &data->iA_seq.pos.real , &accum->iA_seq.pos.real[0] );
	update_AvgMaxMin_1_int32( &data->iA_seq.pos.imag , &accum->iA_seq.pos.imag[0] );
	// negative sequence
	update_AvgMaxMin_1_int32( &data->iA_seq.neg.real , &accum->iA_seq.neg.real[0] );
	update_AvgMaxMin_1_int32( &data->iA_seq.neg.imag , &accum->iA_seq.neg.imag[0] );
	// homopolar sequence
	update_AvgMaxMin_1_int32( &data->iA_seq.hom.real , &accum->iA_seq.hom.real[0] );
	update_AvgMaxMin_1_int32( &data->iA_seq.hom.imag , &accum->iA_seq.hom.imag[0] );

	// Update current B
	// positive sequence
	update_AvgMaxMin_1_int32( &data->iB_seq.pos.real , &accum->iB_seq.pos.real[0] );
	update_AvgMaxMin_1_int32( &data->iB_seq.pos.imag , &accum->iB_seq.pos.imag[0] );
	// negative sequence
	update_AvgMaxMin_1_int32( &data->iB_seq.neg.real , &accum->iB_seq.neg.real[0] );
	update_AvgMaxMin_1_int32( &data->iB_seq.neg.imag , &accum->iB_seq.neg.imag[0] );
	// homopolar sequence
	update_AvgMaxMin_1_int32( &data->iB_seq.hom.real , &accum->iB_seq.hom.real[0] );
	update_AvgMaxMin_1_int32( &data->iB_seq.hom.imag , &accum->iB_seq.hom.imag[0] );

	// compute desbalance
	update_AvgMaxMin_1_int32( &data->v_desb_neg , &accum->v_desb_neg[0] );
	update_AvgMaxMin_1_int32( &data->v_desb_hom , &accum->v_desb_hom[0] );
	update_AvgMaxMin_1_int32( &data->iA_desb_neg , &accum->iA_desb_neg[0] );
	update_AvgMaxMin_1_int32( &data->iA_desb_hom , &accum->iA_desb_hom[0] );
	update_AvgMaxMin_1_int32( &data->iB_desb_neg , &accum->iB_desb_neg[0] );
	update_AvgMaxMin_1_int32( &data->iB_desb_hom , &accum->iB_desb_hom[0] );

	// Update power
	{
		uint16_t CH_count;

		for ( CH_count=0 ; CH_count<CH_POWER_TOTAL_NUM ; CH_count++ ) {

			update_AvgMaxMin_1_int32( &data->ch_power[CH_count].ApparentPower , &accum->ch_power[CH_count].ApparentPower[0] );
			update_AvgMaxMin_1_int32( &data->ch_power[CH_count].ActivePower   , &accum->ch_power[CH_count].ActivePower[0]   );
			update_AvgMaxMin_1_int32( &data->ch_power[CH_count].NoactivePower , &accum->ch_power[CH_count].NoactivePower[0] );
			update_AvgMaxMin_1_int32( &data->ch_power[CH_count].PowerFactor   , &accum->ch_power[CH_count].PowerFactor[0]   );

		}

	}

}

/// @brief Update accumulator with index data
void trend_update_accum( trend_t * data , trend_accum_t * accum ) {

	// increase accumulator
	accum->accum++;

	// update flags
	accum->flags |= data->flags;

	// maximum time utilization
	accum->time_utilization = MAX( data->time_utilization , accum->time_utilization );

	// update sampling period
	update_AvgMaxMin_2_int32( &data->Ts[0] , &accum->Ts[0] );

	// frequency
	update_AvgMaxMin_2_int32( &data->fL[0] , &accum->fL[0] );

	// update RMS and harmonics accumulator
	{
		uint16_t CH_count;
		uint16_t H_count;

		for ( CH_count=0 ; CH_count<CH_ADC_GRID_TOTAL_NUM ; CH_count++ ) {

			int32_t temp_square[3];

			// RMS
			temp_square[0] = SQUARE( (int32_t)data->ch[CH_count].rms[0] );
			temp_square[1] = SQUARE( (int32_t)data->ch[CH_count].rms[1] );
			temp_square[2] = SQUARE( (int32_t)data->ch[CH_count].rms[2] );
			update_AvgMaxMin_2_int64( &temp_square[0] , &accum->ch[CH_count].rms2[0] );

			// Harmonics
			for ( H_count=0 ; H_count<(SETTINGS_DSP_NDFT_MAX+1) ; H_count++ ) {

				update_AvgMaxMin_2_int32( &data->ch[CH_count].harmonic[H_count].real[0] , &accum->ch[CH_count].harmonic[H_count].real[0] );
				update_AvgMaxMin_2_int32( &data->ch[CH_count].harmonic[H_count].imag[0] , &accum->ch[CH_count].harmonic[H_count].imag[0] );

			}

			// Total Waveform Distortion (TWD)
			temp_square[0] = SQUARE( (int32_t)data->ch[CH_count].twd[0] );
			temp_square[1] = SQUARE( (int32_t)data->ch[CH_count].twd[1] );
			temp_square[2] = SQUARE( (int32_t)data->ch[CH_count].twd[2] );
			update_AvgMaxMin_2_int64( &temp_square[0] , &accum->ch[CH_count].twd2[0] );

		}

	}

	// Update voltage
	// positive sequence
	update_AvgMaxMin_2_int32( &data->v_seq.pos.real[0] , &accum->v_seq.pos.real[0] );
	update_AvgMaxMin_2_int32( &data->v_seq.pos.imag[0] , &accum->v_seq.pos.imag[0] );
	// negative sequence
	update_AvgMaxMin_2_int32( &data->v_seq.neg.real[0] , &accum->v_seq.neg.real[0] );
	update_AvgMaxMin_2_int32( &data->v_seq.neg.imag[0] , &accum->v_seq.neg.imag[0] );
	// homopolar sequence
	update_AvgMaxMin_2_int32( &data->v_seq.hom.real[0] , &accum->v_seq.hom.real[0] );
	update_AvgMaxMin_2_int32( &data->v_seq.hom.imag[0] , &accum->v_seq.hom.imag[0] );

	// Update current A
	// positive sequence
	update_AvgMaxMin_2_int32( &data->iA_seq.pos.real[0] , &accum->iA_seq.pos.real[0] );
	update_AvgMaxMin_2_int32( &data->iA_seq.pos.imag[0] , &accum->iA_seq.pos.imag[0] );
	// negative sequence
	update_AvgMaxMin_2_int32( &data->iA_seq.neg.real[0] , &accum->iA_seq.neg.real[0] );
	update_AvgMaxMin_2_int32( &data->iA_seq.neg.imag[0] , &accum->iA_seq.neg.imag[0] );
	// homopolar sequence
	update_AvgMaxMin_2_int32( &data->iA_seq.hom.real[0] , &accum->iA_seq.hom.real[0] );
	update_AvgMaxMin_2_int32( &data->iA_seq.hom.imag[0] , &accum->iA_seq.hom.imag[0] );

	// Update current B
	// positive sequence
	update_AvgMaxMin_2_int32( &data->iB_seq.pos.real[0] , &accum->iB_seq.pos.real[0] );
	update_AvgMaxMin_2_int32( &data->iB_seq.pos.imag[0] , &accum->iB_seq.pos.imag[0] );
	// negative sequence
	update_AvgMaxMin_2_int32( &data->iB_seq.neg.real[0] , &accum->iB_seq.neg.real[0] );
	update_AvgMaxMin_2_int32( &data->iB_seq.neg.imag[0] , &accum->iB_seq.neg.imag[0] );
	// homopolar sequence
	update_AvgMaxMin_2_int32( &data->iB_seq.hom.real[0] , &accum->iB_seq.hom.real[0] );
	update_AvgMaxMin_2_int32( &data->iB_seq.hom.imag[0] , &accum->iB_seq.hom.imag[0] );

	// compute desbalance
	update_AvgMaxMin_2_int32( &data->v_desb_neg[0] , &accum->v_desb_neg[0] );
	update_AvgMaxMin_2_int32( &data->v_desb_hom[0] , &accum->v_desb_hom[0] );
	update_AvgMaxMin_2_int32( &data->iA_desb_neg[0] , &accum->iA_desb_neg[0] );
	update_AvgMaxMin_2_int32( &data->iA_desb_hom[0] , &accum->iA_desb_hom[0] );
	update_AvgMaxMin_2_int32( &data->iB_desb_neg[0] , &accum->iB_desb_neg[0] );
	update_AvgMaxMin_2_int32( &data->iB_desb_hom[0] , &accum->iB_desb_hom[0] );

	// Update power
	{
		uint16_t CH_count;

		for ( CH_count=0 ; CH_count<CH_POWER_TOTAL_NUM ; CH_count++ ) {

			update_AvgMaxMin_2_int32( &data->ch_power[CH_count].ApparentPower[0] , &accum->ch_power[CH_count].ApparentPower[0] );
			update_AvgMaxMin_2_int32( &data->ch_power[CH_count].ActivePower[0]   , &accum->ch_power[CH_count].ActivePower[0] );
			update_AvgMaxMin_2_int32( &data->ch_power[CH_count].NoactivePower[0] , &accum->ch_power[CH_count].NoactivePower[0] );
			update_AvgMaxMin_2_int32( &data->ch_power[CH_count].PowerFactor[0]   , &accum->ch_power[CH_count].PowerFactor[0] );

		}

	}

}

void trend_update_result( trend_accum_t * accum , trend_t * result ) {


	// get time
	copy_content( (uint8_t *)rtc_get_time(),
			      (uint8_t *)&result->time,
				  sizeof(rtc_t) );

	// update flags
	result->flags = accum->flags;

	// maximum time utilization
	result->time_utilization = accum->time_utilization;

	// update sampling period
	set_AvgMaxMin_int32( &accum->Ts[0] , &result->Ts[0] , accum->accum );

	// frequency
	set_AvgMaxMin_int32( &accum->fL[0] , &result->fL[0] , accum->accum );

	// update RMS and harmonics accumulator
	{
		uint16_t CH_count;
		uint16_t H_count;

		for ( CH_count=0 ; CH_count<CH_ADC_GRID_TOTAL_NUM ; CH_count++ ) {

			// RMS
			set_AvgMaxMin_int64( &accum->ch[CH_count].rms2[0] , &result->ch[CH_count].rms[0] , accum->accum );

			// Harmonics
			for ( H_count=0 ; H_count<(SETTINGS_DSP_NDFT_MAX+1) ; H_count++ ) {

				set_AvgMaxMin_int32( &accum->ch[CH_count].harmonic[H_count].real[0] , &result->ch[CH_count].harmonic[H_count].real[0]   , accum->accum);
				set_AvgMaxMin_int32( &accum->ch[CH_count].harmonic[H_count].imag[0] , &result->ch[CH_count].harmonic[H_count].imag[0] , accum->accum);

			}

			// Total Waveform Distortion (TWD)
			set_AvgMaxMin_int64( &accum->ch[CH_count].twd2[0] , &result->ch[CH_count].twd[0] , accum->accum );
		}

	}

	// Update voltage
	// positive sequence
	set_AvgMaxMin_int32( &accum->v_seq.pos.real[0] , &result->v_seq.pos.real[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->v_seq.pos.imag[0] , &result->v_seq.pos.imag[0] , accum->accum);
	// negative sequence
	set_AvgMaxMin_int32( &accum->v_seq.neg.real[0] , &result->v_seq.neg.real[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->v_seq.neg.imag[0] , &result->v_seq.neg.imag[0] , accum->accum);
	// homopolar sequence
	set_AvgMaxMin_int32( &accum->v_seq.hom.real[0] , &result->v_seq.hom.real[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->v_seq.hom.imag[0] , &result->v_seq.hom.imag[0] , accum->accum);

	// Update current
	// positive sequence A
	set_AvgMaxMin_int32( &accum->iA_seq.pos.real[0] , &result->iA_seq.pos.real[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->iA_seq.pos.imag[0] , &result->iA_seq.pos.imag[0] , accum->accum);
	// negative sequence
	set_AvgMaxMin_int32( &accum->iA_seq.neg.real[0] , &result->iA_seq.neg.real[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->iA_seq.neg.imag[0] , &result->iA_seq.neg.imag[0] , accum->accum);
	// homopolar sequence
	set_AvgMaxMin_int32( &accum->iA_seq.hom.real[0] , &result->iA_seq.hom.real[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->iA_seq.hom.imag[0] , &result->iA_seq.hom.imag[0] , accum->accum);

	// Update current B
	// positive sequence
	set_AvgMaxMin_int32( &accum->iB_seq.pos.real[0] , &result->iB_seq.pos.real[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->iB_seq.pos.imag[0] , &result->iB_seq.pos.imag[0] , accum->accum);
	// negative sequence
	set_AvgMaxMin_int32( &accum->iB_seq.neg.real[0] , &result->iB_seq.neg.real[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->iB_seq.neg.imag[0] , &result->iB_seq.neg.imag[0] , accum->accum);
	// homopolar sequence
	set_AvgMaxMin_int32( &accum->iB_seq.hom.real[0] , &result->iB_seq.hom.real[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->iB_seq.hom.imag[0] , &result->iB_seq.hom.imag[0] , accum->accum);

	// compute desbalance
	set_AvgMaxMin_int32( &accum->v_desb_neg[0]  , &result->v_desb_neg[0]  , accum->accum);
	set_AvgMaxMin_int32( &accum->v_desb_hom[0]  , &result->v_desb_hom[0]  , accum->accum);
	set_AvgMaxMin_int32( &accum->iA_desb_neg[0] , &result->iA_desb_neg[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->iA_desb_hom[0] , &result->iA_desb_hom[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->iB_desb_neg[0] , &result->iB_desb_neg[0] , accum->accum);
	set_AvgMaxMin_int32( &accum->iB_desb_hom[0] , &result->iB_desb_hom[0] , accum->accum);

	// Update power
	{
		uint16_t CH_count;

		for ( CH_count=0 ; CH_count<CH_POWER_TOTAL_NUM ; CH_count++ ) {

			set_AvgMaxMin_int32( &accum->ch_power[CH_count].ApparentPower[0] , &result->ch_power[CH_count].ApparentPower[0] , accum->accum);
			set_AvgMaxMin_int32( &accum->ch_power[CH_count].ActivePower[0]   , &result->ch_power[CH_count].ActivePower[0]   , accum->accum);
			set_AvgMaxMin_int32( &accum->ch_power[CH_count].NoactivePower[0] , &result->ch_power[CH_count].NoactivePower[0] , accum->accum);
			set_AvgMaxMin_int32( &accum->ch_power[CH_count].PowerFactor[0]   , &result->ch_power[CH_count].PowerFactor[0]   , accum->accum);

		}

	}

	// clear accumulator
	trend_reset_accum( accum );

}

/// @}

