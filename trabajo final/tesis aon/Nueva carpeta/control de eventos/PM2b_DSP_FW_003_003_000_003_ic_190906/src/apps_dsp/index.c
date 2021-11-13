// $Header
/// @file			index.c
/// @brief  		Index generation file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		07 November 2016
/// @details
/// @copyright
///

/// @addtogroup apps
/// @{

/**************************** INCLUDES ***************************************/

#include "index.h"
#include "../utils/iqmath.h"
#include "../utils/calculus.h"
#include "../apps/control.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

int16_t compute_mod( index_complex16_t * in ) {

	return sqrt( SQUARE((int32_t)in->real) + SQUARE((int32_t)in->imag) );

}

int16_t compute_percentage( int16_t * num , int16_t * den ) {

	return (((int32_t)*num << 12) / *den);

}


/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Update accumulator with processed data
void index_update_accum( sp_t * data , index_accum_t * accum ) {

	// increase accumulator
	accum->accum++;

	// update sampling period
	accum->Ts += data->Ts;

	// update RMS and harmonics accumulator
	{
		uint16_t CH_count;
		uint16_t H_count;

		for ( CH_count=0 ; CH_count<CH_ADC_GRID_TOTAL_NUM ; CH_count++ ) {

			// RMS
			accum->ch[CH_count].rms2 += (data->ch[CH_count].rms2);

			// Harmonics
			for ( H_count=0 ; H_count<(SETTINGS_DSP_NDFT_MAX+1) ; H_count++ ) {
				accum->ch[CH_count].harmonic[H_count].real += (data->ch[CH_count].harmonic[H_count].real>>18);
				accum->ch[CH_count].harmonic[H_count].imag += (data->ch[CH_count].harmonic[H_count].imag>>18);
			}
		}

	}

	// Update voltage
	// positive sequence
	accum->v_seq.pos.real += data->v_seq.pos.real;
	accum->v_seq.pos.imag += data->v_seq.pos.imag;
	// negative sequence
	accum->v_seq.neg.real += data->v_seq.neg.real;
	accum->v_seq.neg.imag += data->v_seq.neg.imag;
	// homopolar sequence
	accum->v_seq.hom.real += data->v_seq.hom.real;
	accum->v_seq.hom.imag += data->v_seq.hom.imag;

	// Update current A
	// positive sequence
	accum->iA_seq.pos.real += data->iA_seq.pos.real;
	accum->iA_seq.pos.imag += data->iA_seq.pos.imag;
	// negative sequence
	accum->iA_seq.neg.real += data->iA_seq.neg.real;
	accum->iA_seq.neg.imag += data->iA_seq.neg.imag;
	// homopolar sequence
	accum->iA_seq.hom.real += data->iA_seq.hom.real;
	accum->iA_seq.hom.imag += data->iA_seq.hom.imag;

	// Update current B
	// positive sequence
	accum->iB_seq.pos.real += data->iB_seq.pos.real;
	accum->iB_seq.pos.imag += data->iB_seq.pos.imag;
	// negative sequence
	accum->iB_seq.neg.real += data->iB_seq.neg.real;
	accum->iB_seq.neg.imag += data->iB_seq.neg.imag;
	// homopolar sequence
	accum->iB_seq.hom.real += data->iB_seq.hom.real;
	accum->iB_seq.hom.imag += data->iB_seq.hom.imag;

	// Update instantaneous power accumulator
	accum->inst_power[CH_P1A] += data->inst_power[CH_P1A];
	accum->inst_power[CH_P2A] += data->inst_power[CH_P2A];
	accum->inst_power[CH_P3A] += data->inst_power[CH_P3A];
	accum->inst_power[CH_P1B] += data->inst_power[CH_P1B];
	accum->inst_power[CH_P2B] += data->inst_power[CH_P2B];
	accum->inst_power[CH_P3B] += data->inst_power[CH_P3B];

}


/// @brief Update index result
void index_update_result( index_accum_t * accum , index_result_t * result ) {

	// update sampling period
	result->Ts = accum->Ts / accum->accum;

	// maximum time utilization
	result->time_utilization = accum->time_utilization;

	// frequency
	result->fL = ((int32_t)(SETTINGS_DSP_SYSCLK_HZ*Q08/SETTINGS_DSP_NPLL)) / result->Ts;

	// update trend flag
	result->flags = *ctrl_trend_flag_temp_ptr(); // save
	*ctrl_trend_flag_temp_ptr() = 0;			// clear


	// update RMS and harmonics accumulator
	{
		uint16_t CH_count;
		uint16_t H_count;

		for ( CH_count=0 ; CH_count<CH_ADC_GRID_TOTAL_NUM ; CH_count++ ) {

			int32_t rms2;

			// RMS
			rms2 = ((accum->ch[CH_count].rms2)>>7) / accum->accum;
			result->ch[CH_count].rms = sqrt( rms2 );

			// Harmonics
			for ( H_count=0 ; H_count<(SETTINGS_DSP_NDFT_MAX+1) ; H_count++ ) {
				result->ch[CH_count].harmonic[H_count].real = accum->ch[CH_count].harmonic[H_count].real / accum->accum;
				result->ch[CH_count].harmonic[H_count].imag = accum->ch[CH_count].harmonic[H_count].imag / accum->accum;
			}

			// Total Waveform Distortion (TWD)
			//  Equation 1 -> TWD  = sqrt(rms^2 - H1rms^2)/H1rms
			//  Equation 2 -> TWD2 = (rms^2 - H1rms^2)/V1rms^2
			{
	//			int32_t H1rms2 = SQUARE( (int32_t)result->ch[CH_count].harmonic[1].mod ) >> 1;
				int32_t H1rms2 = SQUARE( (int32_t)compute_mod(&result->ch[CH_count].harmonic[1]) ) >> 1;
				int64_t twd2_temp;

				twd2_temp = rms2;
				twd2_temp -= H1rms2;
				if (twd2_temp<0)
					twd2_temp = 0;
				twd2_temp = twd2_temp<<24;
				result->ch[CH_count].twd = sqrt( twd2_temp / H1rms2 );

			}
		}

	}

	// Update voltage
	// positive sequence
	result->v_seq.pos.real = (accum->v_seq.pos.real / accum->accum) / 3;
	result->v_seq.pos.imag = (accum->v_seq.pos.imag / accum->accum) / 3;
	// negative sequence
	result->v_seq.neg.real = (accum->v_seq.neg.real / accum->accum) / 3;
	result->v_seq.neg.imag = (accum->v_seq.neg.imag / accum->accum) / 3;
	// homopolar sequence
	result->v_seq.hom.real = (accum->v_seq.hom.real / accum->accum) / 3;
	result->v_seq.hom.imag = (accum->v_seq.hom.imag / accum->accum) / 3;

	// Update current A
	// positive sequence
	result->iA_seq.pos.real = (accum->iA_seq.pos.real / accum->accum) / 3;
	result->iA_seq.pos.imag = (accum->iA_seq.pos.imag / accum->accum) / 3;
	// negative sequence
	result->iA_seq.neg.real = (accum->iA_seq.neg.real / accum->accum) / 3;
	result->iA_seq.neg.imag = (accum->iA_seq.neg.imag / accum->accum) / 3;
	// homopolar sequence
	result->iA_seq.hom.real = (accum->iA_seq.hom.real / accum->accum) / 3;
	result->iA_seq.hom.imag = (accum->iA_seq.hom.imag / accum->accum) / 3;

	// Update current B
	// positive sequence
	result->iB_seq.pos.real = (accum->iB_seq.pos.real / accum->accum) / 3;
	result->iB_seq.pos.imag = (accum->iB_seq.pos.imag / accum->accum) / 3;
	// negative sequence
	result->iB_seq.neg.real = (accum->iB_seq.neg.real / accum->accum) / 3;
	result->iB_seq.neg.imag = (accum->iB_seq.neg.imag / accum->accum) / 3;
	// homopolar sequence
	result->iB_seq.hom.real = (accum->iB_seq.hom.real / accum->accum) / 3;
	result->iB_seq.hom.imag = (accum->iB_seq.hom.imag / accum->accum) / 3;

	// compute desbalance
	{
		int16_t seq_pos_mod;
		int16_t seq_neg_mod;
		int16_t seq_hom_mod;

		// Voltage
		seq_pos_mod = compute_mod(&result->v_seq.pos);
		seq_neg_mod = compute_mod(&result->v_seq.neg);
		seq_hom_mod = compute_mod(&result->v_seq.hom);
		result->v_desb_neg = compute_percentage( &seq_neg_mod , &seq_pos_mod );
		result->v_desb_hom = compute_percentage( &seq_hom_mod , &seq_pos_mod );

		// current iA
		seq_pos_mod = compute_mod(&result->iA_seq.pos);
		seq_neg_mod = compute_mod(&result->iA_seq.neg);
		seq_hom_mod = compute_mod(&result->iA_seq.hom);
		result->iA_desb_neg = compute_percentage( &seq_neg_mod , &seq_pos_mod );
		result->iA_desb_hom = compute_percentage( &seq_hom_mod , &seq_pos_mod );

		// current iB
		seq_pos_mod = compute_mod(&result->iB_seq.pos);
		seq_neg_mod = compute_mod(&result->iB_seq.neg);
		seq_hom_mod = compute_mod(&result->iB_seq.hom);
		result->iB_desb_neg = compute_percentage( &seq_neg_mod , &seq_pos_mod );
		result->iB_desb_hom = compute_percentage( &seq_hom_mod , &seq_pos_mod );

	}

	// Update power
	{
		uint16_t CH_count;

		for ( CH_count=0 ; CH_count<CH_POWER_TOTAL_NUM ; CH_count++ ) {

			result->ch_power[CH_count].ActivePower = (accum->inst_power[CH_count]>>(7+14)) / accum->accum;
			if (CH_count < (CH_POWER_TOTAL_NUM>>1))
				result->ch_power[CH_count].ApparentPower = FPMUL( result->ch[CH_count].rms , result->ch[CH_count+4].rms , 14 );
			else
				result->ch_power[CH_count].ApparentPower = FPMUL( result->ch[CH_count-3].rms , result->ch[CH_count+5].rms , 14 );
			result->ch_power[CH_count].NoactivePower = sqrt( SQUARE((int32_t)result->ch_power[CH_count].ApparentPower) - SQUARE((int32_t)result->ch_power[CH_count].ActivePower) );
			result->ch_power[CH_count].PowerFactor = (((int32_t)result->ch_power[CH_count].ActivePower)<<12) / result->ch_power[CH_count].ApparentPower;

		}

	}

	// clear accumulator
	memset( (void *)accum , 0 , sizeof(index_accum_t) );

}

/// @}

