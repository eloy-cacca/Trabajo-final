// $Header
/// @file			signal_processing.c
/// @brief  		Signal processing file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		24 October 2016
/// @details
/// @copyright
///

/// @addtogroup Modules
/// @{

/**************************** INCLUDES ***************************************/

#include "signal_processing.h"
#include "../apps/system.h"
#include "../utils/iqmath.h"
#include "../utils/lut.h"

/**************************** MACROS *****************************************/

#define C2 TOFIX( 0.866025403784439 , 15 )  // sqrt(3)/2 * 1/3

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

// ISC
//
// Calculus:
//   | Vpos_real + j*Vpos_imag |         | 1 a   a^2 | |Va_real + j*Va_imag|
//   | Vneg_real + j*Vneg_imag | = 0.333*| 1 a^2 a   |*|Vb_real + j*Vb_imag|
//   | Vhom_real + j*Vhom_imag |         | 1 1   1   | |Vc_real + j*Vc_imag|
// where
//   a = exp(j*2*pi/3) = -0.5 + j*0.866
//
// Implementation:
//   Vpos_real = 0.333 * ( Va_real - 0.5*(Vb_real + Vc_real) + 0.866*(-Vb_imag + Vc_imag) )
//   Vpos_imag = 0.333 * ( Va_imag - 0.5*(Vb_imag + Vc_imag) + 0.866*(+Vb_real - Vc_real) )
//   Vneg_real = 0.333 * ( Va_real - 0.5*(Vb_real + Vc_real) - 0.866*(-Vb_imag + Vc_imag) )
//   Vneg_imag = 0.333 * ( Va_imag - 0.5*(Vb_imag + Vc_imag) - 0.866*(+Vb_real - Vc_real) )
//   Vhom_real = 0.333 * ( Va_real + Vb_real + Vc_real )
//   Vhom_imag = 0.333 * ( Va_imag + Vb_imag + Vc_imag )
//
void isc_calculus( sp_complex_t * s1 , sp_complex_t * s2 , sp_complex_t * s3 , sp_seq_t * seq ) {

	int32 Xar = s1->real>>18;
	int32 Xai = s1->imag>>18;
	int32 Xbr = s2->real>>18;
	int32 Xbi = s2->imag>>18;
	int32 Xcr = s3->real>>18;
	int32 Xci = s3->imag>>18;

	int32 temp1 = Xar - (( Xbr + Xcr )>>1);
	int32 temp2 = FPMUL( Xci - Xbi , C2 , 15 );
	int32 temp3 = Xai - (( Xbi + Xci )>>1);
	int32 temp4 = FPMUL( Xbr - Xcr , C2 , 15 );

	seq->pos.real = temp1 + temp2;
	seq->pos.imag = temp3 + temp4;
	seq->neg.real = temp1 - temp2;
	seq->neg.imag = temp3 - temp4;
	seq->hom.real = Xar + Xbr + Xcr;
	seq->hom.imag = Xai + Xbi + Xci;

}

void msdft_modulation( uint16_t H , uint16_t * index , int32_t * input , sp_complex_t * value ) {

	// mDFT Index
	*index += H;
	if (*index>(SETTINGS_DSP_NPLL-1)) {*index -= SETTINGS_DSP_NPLL;}

	// Modulator and Resonator
	// NOTE: Output is in Q(7 - 1 + 12) = Q18
	//  Gain of SW is Q7
	//  Gain of modulation 1/2
	//  Sine and cosine function are in Q12 (Q20>>8=Q12)
	value->real += (*input) * (lut_cos_128_Q20( *index )>>8);
	value->imag -= (*input) * (lut_sin_128_Q20( *index )>>8);

}

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

void processing_update( int32_t * signal , sp_t * data ) {

	Uint16 CH_count; // channel counter

	// Update sampling frequency
#if SETTINGS_ISR_TYPE
	data->Ts = cputimer0_set_period;
#else
	data->Ts = epwm1_set_period;
#endif

	// Update accumulator of instantaneous power.
	// NOTE: It must be implemented before sliding window update.
	for ( CH_count=0 ; CH_count<(CH_POWER_TOTAL_NUM>>1) ; CH_count++ ) {

		// A
		data->inst_power[CH_count] += FPMUL( signal[CH_count] , signal[CH_count+4] , 0 );
		data->inst_power[CH_count] -= FPMUL( data->ch[CH_count].buffer[data->ch[CH_count].buffer_index] , data->ch[CH_count+4].buffer[data->ch[CH_count+4].buffer_index] , 0 );

		// B
		data->inst_power[CH_count+3] += FPMUL( signal[CH_count] , signal[CH_count+8] , 0 );
		data->inst_power[CH_count+3] -= FPMUL( data->ch[CH_count].buffer[data->ch[CH_count].buffer_index] , data->ch[CH_count+8].buffer[data->ch[CH_count+8].buffer_index] , 0 );

	}

	// Update channel parameters
	for ( CH_count=0 ; CH_count<CH_ADC_GRID_TOTAL_NUM ; CH_count++ ) {

		int32_t SW_out;
		int32_t * input = &signal[CH_count];
		sp_channel_t * ch = &data->ch[CH_count];
		int32_t * signal_last_N = &data->ch[CH_count].buffer[data->ch[CH_count].buffer_index];

		// Update accumulator of signal^2 (used for RMS calculation)
		ch->rms2 += FPMUL( *input , *input , 0 );
		ch->rms2 -= FPMUL( *signal_last_N , *signal_last_N , 0 );


		// Update Sliding window output
		SW_out = *input - *signal_last_N;

		// Update sliding window buffer and index
		ch->buffer[ch->buffer_index] = *input;
		ch->buffer_index++;
		if (ch->buffer_index>(SETTINGS_DSP_NPLL-1))
			ch->buffer_index = 0;

		// Update DC accumulator (MAF). Harmonics are stored in Q18 so data is shifted by 11. With SW length (<<7) it results in Q18
		ch->harmonic[0].real += (SW_out<<11);
		ch->harmonic[0].imag = 0;

		// Update harmonics accumulators (Modulated Sliding DFT)
		msdft_modulation( 1 , &ch->harmonic_index[1] , &SW_out , &ch->harmonic[1] );
		msdft_modulation( 3 , &ch->harmonic_index[2] , &SW_out , &ch->harmonic[2] );
		msdft_modulation( 5 , &ch->harmonic_index[3] , &SW_out , &ch->harmonic[3] );
		msdft_modulation( 7 , &ch->harmonic_index[4] , &SW_out , &ch->harmonic[4] );

	}

	// ISC
	isc_calculus( &data->ch[CH_ADC_V1_N].harmonic[1] ,
			      &data->ch[CH_ADC_V2_N].harmonic[1] ,
			      &data->ch[CH_ADC_V3_N].harmonic[1] ,
			      &data->v_seq );
	isc_calculus( &data->ch[CH_ADC_I1A].harmonic[1] ,
			      &data->ch[CH_ADC_I2A].harmonic[1] ,
			      &data->ch[CH_ADC_I3A].harmonic[1] ,
			      &data->iA_seq );
	isc_calculus( &data->ch[CH_ADC_I1B].harmonic[1] ,
			      &data->ch[CH_ADC_I2B].harmonic[1] ,
			      &data->ch[CH_ADC_I3B].harmonic[1] ,
			      &data->iB_seq );

}

/// @}


