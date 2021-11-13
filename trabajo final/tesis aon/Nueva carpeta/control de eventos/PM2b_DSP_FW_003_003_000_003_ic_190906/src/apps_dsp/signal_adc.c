// $Header
/// @file			signal_adc.c
/// @brief  		Signal ADC file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		08 November 2016
/// @details
/// @copyright
///

/// @addtogroup Modules
/// @{

/**************************** INCLUDES ***************************************/

#include "signal_adc.h"

#include "../apps/unit_config.h"
#include "../utils/iqmath.h"
#include "../utils/lut.h"
#include <stdlib.h>

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief ADC Sensor update.
/// @details Input signals are acquired and converter to engineering values
/// @param[inout] data_ptr Pointer to input data structure. Return signals
void signal_adc_update(int32_t *data_ptr) {

	// Save signal and compensate offset and gain
	if ( (unit_config.mode_flags & UNIT_CONFIG_MODE_ADC_MASK) ==  UNIT_CONFIG_MODE_ADC_EMUL ) {

#if 0

#define UPDATE_INDEX(A,B)   { A+=B;  if (A>(LUT_SIN_128_N-1))	A-=LUT_SIN_128_N;}

		// voltage H1
		static uint16_t VL1_phase_H1 = 128*(360.0/360.0);
		static uint16_t VL2_phase_H1 = 128*(241.0/360.0);
		static uint16_t VL3_phase_H1 = 128*(120.0/360.0);
		UPDATE_INDEX(VL1_phase_H1,1);
		UPDATE_INDEX(VL2_phase_H1,1);
		UPDATE_INDEX(VL3_phase_H1,1);
		// voltage H5
		static uint16_t VL1_phase_H5 = 50;
		static uint16_t VL2_phase_H5 = 110;
		static uint16_t VL3_phase_H5 = 50;
		UPDATE_INDEX(VL1_phase_H5,5);
		UPDATE_INDEX(VL2_phase_H5,5);
		UPDATE_INDEX(VL3_phase_H5,5);
		// voltage H7
		static uint16_t VL1_phase_H7 = 150;
		static uint16_t VL2_phase_H7 = 30;
		static uint16_t VL3_phase_H7 = 240;
		UPDATE_INDEX(VL1_phase_H7,7);
		UPDATE_INDEX(VL2_phase_H7,7);
		UPDATE_INDEX(VL3_phase_H7,7);
		// current H1
		static uint16_t IL1_phase_H1 = 128*(336.0/360.0);
		static uint16_t IL2_phase_H1 = 128*(216.0/360.0);
		static uint16_t IL3_phase_H1 = 128*( 90.0/360.0);
		UPDATE_INDEX(IL1_phase_H1,1);
		UPDATE_INDEX(IL2_phase_H1,1);
		UPDATE_INDEX(IL3_phase_H1,1);
		// current H5
		static uint16_t IL1_phase_H5 = 0 + 200;
		static uint16_t IL2_phase_H5 = 85 + 200;
		static uint16_t IL3_phase_H5 = 43 + 200;
		UPDATE_INDEX(IL1_phase_H5,5);
		UPDATE_INDEX(IL2_phase_H5,5);
		UPDATE_INDEX(IL3_phase_H5,5);

		static uint16_t VL1_variation_period = 1000;
		static uint16_t VL1_variation_count = 0;
		static float VL1_var = 0;
		VL1_variation_count++;
		if ( VL1_variation_count > VL1_variation_period ) {
			VL1_variation_period = rand();
			VL1_variation_count = 0;
//			VL1_var = (10.0/400.0)*(rand()/32768.0);
		}

		static uint16_t VL2_variation_period = 1000;
		static uint16_t VL2_variation_count = 0;
		static float VL2_var = 0;
		VL2_variation_count++;
		if ( VL2_variation_count > VL2_variation_period ) {
			VL2_variation_period = rand();
			VL2_variation_count = 0;
			VL2_var = (10.0/400.0)*(rand()/32768.0);
		}

		static uint16_t VL3_variation_period = 1000;
		static uint16_t VL3_variation_count = 0;
		static float VL3_var = 0;
		VL3_variation_count++;
		if ( VL3_variation_count > VL3_variation_period ) {
			VL3_variation_period = rand();
			VL3_variation_count = 0;
			VL3_var = (10.0/400.0)*(rand()/32768.0);
		}

		// voltage (400v full scale)
		data_ptr[CH_V1] = (int32)( ((310.0/400.0)+VL1_var)*(lut_cos_128_Q20( VL1_phase_H1 )>>5) + ( 13.0/400.0)*(lut_cos_128_Q20( VL1_phase_H5 )>>5) + ( 10.0/400.0)*(lut_cos_128_Q20( VL1_phase_H7 )>>5)  );
		data_ptr[CH_V2] = (int32)( ((305.0/400.0)+VL2_var)*(lut_cos_128_Q20( VL2_phase_H1 )>>5) + ( 10.0/400.0)*(lut_cos_128_Q20( VL2_phase_H5 )>>5) + (  5.0/400.0)*(lut_cos_128_Q20( VL2_phase_H7 )>>5)  );
		data_ptr[CH_V3] = (int32)( ((318.0/400.0)+VL3_var)*(lut_cos_128_Q20( VL3_phase_H1 )>>5) + ( 15.0/400.0)*(lut_cos_128_Q20( VL3_phase_H5 )>>5) + (  5.0/400.0)*(lut_cos_128_Q20( VL3_phase_H7 )>>5)  );
		data_ptr[CH_VE] = 0;
		// current (200A full scale)
		data_ptr[CH_I1] = (int32)(  (89.0/200.0)*(lut_cos_128_Q20( IL1_phase_H1 )>>5) + (20.0/200.0)*(lut_cos_128_Q20( IL1_phase_H5 )>>5) );
		data_ptr[CH_I2] = (int32)( (108.0/200.0)*(lut_cos_128_Q20( IL2_phase_H1 )>>5) + (10.0/200.0)*(lut_cos_128_Q20( IL2_phase_H5 )>>5) );
		data_ptr[CH_I3] = (int32)( (125.0/200.0)*(lut_cos_128_Q20( IL3_phase_H1 )>>5) + (50.0/200.0)*(lut_cos_128_Q20( IL3_phase_H5 )>>5) );
		data_ptr[CH_IN] = 0;


#else

		static uint16_t v_phaseA = 0;
		static uint16_t v_phaseB = 85;
		static uint16_t v_phaseC = 43;
		static uint16_t i_phaseA = 0  - 20 + 128;
		static uint16_t i_phaseB = 85 - 20;
		static uint16_t i_phaseC = 43 - 20;
		static uint16_t phaseH1 = 50;
		static uint16_t phaseH2 = 110;
		static uint32_t SAG_index=0;

		// update pahse index
		v_phaseA ++;	if (v_phaseA>(LUT_SIN_128_N-1))	v_phaseA -= LUT_SIN_128_N;
		v_phaseB ++;	if (v_phaseB>(LUT_SIN_128_N-1))	v_phaseB -= LUT_SIN_128_N;
		v_phaseC ++;	if (v_phaseC>(LUT_SIN_128_N-1))	v_phaseC -= LUT_SIN_128_N;
		i_phaseA ++;	if (i_phaseA>(LUT_SIN_128_N-1))	i_phaseA -= LUT_SIN_128_N;
		i_phaseB ++;	if (i_phaseB>(LUT_SIN_128_N-1))	i_phaseB -= LUT_SIN_128_N;
		i_phaseC ++;	if (i_phaseC>(LUT_SIN_128_N-1))	i_phaseC -= LUT_SIN_128_N;
		phaseH1 += 3;	if (phaseH1>(LUT_SIN_128_N-1))	phaseH1 -= LUT_SIN_128_N;
		phaseH2 += 7;	if (phaseH2>(LUT_SIN_128_N-1))	phaseH2 -= LUT_SIN_128_N;

		// update SAG index
		SAG_index++;

		// Emulation of grid voltages and currents
		// IMPORTANT: Look up table values are in Q20, so a shift of -5 is needed in order to emulate a 16 bits ADC
		// voltage (400v full scale)
		#define EVENT_PERIOD    (11520000)  // 30 minutes
//		#define EVENT_PERIOD    (3840000)   // 10 minutes
//		#define EVENT_PERIOD    (384000)    // 1 minute
//		#define EVENT_PERIOD    (192000)    // 30 seconds
//		#define EVENT_PERIOD    (19200)     // 3 seconds
//		#define EVENT_PERIOD	(6400)      // 1 second
//		#define EVENT_DURATION  (6400)      // 1 second
		#define EVENT_DURATION  (640)       // 0.1 seconds
		if ( SAG_index < EVENT_PERIOD ) {
			data_ptr[CH_ADC_V1_N] = (int32)( (311.0/400.0)*(lut_cos_128_Q20( v_phaseA )>>5));
			data_ptr[CH_ADC_V2_N] = (int32)( (311.0/400.0)*(lut_cos_128_Q20( v_phaseB )>>5));
			data_ptr[CH_ADC_V3_N] = (int32)( (311.0/400.0)*(lut_cos_128_Q20( v_phaseC )>>5));
			data_ptr[CH_ADC_VE_N] = 0;
		}
		else if ( SAG_index < (EVENT_PERIOD + EVENT_DURATION) ) {
//			data_ptr[CH_ADC_V1_N] = (int32)( (311.0/400.0)*(lut_cos_128_Q20( v_phaseA )>>5) + ( 70.0/400.0)*(lut_cos_128_Q20( phaseH1 )>>5) + ( 30.0/400.0)*(lut_cos_128_Q20( phaseH2 )>>5));
//			data_ptr[CH_ADC_V2_N] = (int32)( (100.0/400.0)*(lut_cos_128_Q20( v_phaseB )>>5) + ( 90.0/400.0)*(lut_cos_128_Q20( phaseH2 )>>5) + (20.0/400.0)*Q15);
//			data_ptr[CH_ADC_V3_N] = (int32)( ( 50.0/400.0)*(lut_cos_128_Q20( v_phaseC )>>5) + ( 40.0/400.0)*(lut_cos_128_Q20( phaseH1 )>>5) + (30.0/400.0)*Q15);
			data_ptr[CH_ADC_V1_N] = (int32)( (311.0/400.0)*(lut_cos_128_Q20( v_phaseA )>>5));
			data_ptr[CH_ADC_V2_N] = (int32)( (100.0/400.0)*(lut_cos_128_Q20( v_phaseB )>>5));
			data_ptr[CH_ADC_V3_N] = (int32)( ( 50.0/400.0)*(lut_cos_128_Q20( v_phaseC )>>5));
			data_ptr[CH_ADC_VE_N] = 0;
		}
		else {
			SAG_index = 0;
		}
		// current (400A full scale)
		data_ptr[CH_ADC_I1A] = (int32)( (100.0/400.0) * (lut_cos_128_Q20( i_phaseA )>>5) );
		data_ptr[CH_ADC_I2A] = (int32)( (100.0/400.0) * (lut_cos_128_Q20( i_phaseB )>>5) );
		data_ptr[CH_ADC_I3A] = (int32)( (100.0/400.0) * (lut_cos_128_Q20( i_phaseC )>>5) );
		data_ptr[CH_ADC_IN]  = 0;

		// current (400A full scale)
		data_ptr[CH_ADC_I1B] = (int32)( (20.0/400.0) * (lut_cos_128_Q20( i_phaseA )>>5) );
		data_ptr[CH_ADC_I2B] = (int32)( (20.0/400.0) * (lut_cos_128_Q20( i_phaseB )>>5) );
		data_ptr[CH_ADC_I3B] = (int32)( (20.0/400.0) * (lut_cos_128_Q20( i_phaseC )>>5) );
		data_ptr[CH_ADC_IE]  = 0;


#endif

	} else {

		// voltage
		data_ptr[CH_ADC_V1_N] = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_V1_N] , AdcRegs.ADCRESULT1 + unit_calibration.adc_ch_offset[CH_ADC_V1_N] , 15 );
		data_ptr[CH_ADC_V2_N] = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_V2_N] , AdcRegs.ADCRESULT3 + unit_calibration.adc_ch_offset[CH_ADC_V2_N] , 15 );
		data_ptr[CH_ADC_V3_N] = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_V3_N] , AdcRegs.ADCRESULT5 + unit_calibration.adc_ch_offset[CH_ADC_V3_N] , 15 );
		data_ptr[CH_ADC_VE_N] = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_VE_N] , AdcRegs.ADCRESULT7 + unit_calibration.adc_ch_offset[CH_ADC_VE_N] , 15 );

		// current A
		data_ptr[CH_ADC_I1A]  = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_I1A]  , AdcRegs.ADCRESULT0 + unit_calibration.adc_ch_offset[CH_ADC_I1A] , 15 );
		data_ptr[CH_ADC_I2A]  = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_I2A]  , AdcRegs.ADCRESULT2 + unit_calibration.adc_ch_offset[CH_ADC_I2A] , 15 );
		data_ptr[CH_ADC_I3A]  = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_I3A]  , AdcRegs.ADCRESULT4 + unit_calibration.adc_ch_offset[CH_ADC_I3A] , 15 );
		data_ptr[CH_ADC_IN]   = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_IN]   , AdcRegs.ADCRESULT6 + unit_calibration.adc_ch_offset[CH_ADC_IN]  , 15 );

		// current B
		data_ptr[CH_ADC_I1B]  = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_I1B]  , AdcRegs.ADCRESULT0 + unit_calibration.adc_ch_offset[CH_ADC_I1B] , 15 ); // TODO: replace with channel in PM2b
		data_ptr[CH_ADC_I2B]  = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_I2B]  , AdcRegs.ADCRESULT2 + unit_calibration.adc_ch_offset[CH_ADC_I2B] , 15 ); // TODO: replace with channel in PM2b
		data_ptr[CH_ADC_I3B]  = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_I3B]  , AdcRegs.ADCRESULT4 + unit_calibration.adc_ch_offset[CH_ADC_I3B] , 15 ); // TODO: replace with channel in PM2b
		data_ptr[CH_ADC_IE]   = FPMUL( unit_calibration.adc_ch_gain_Q15[CH_ADC_IE]   , AdcRegs.ADCRESULT6 + unit_calibration.adc_ch_offset[CH_ADC_IE]  , 15 ); // TODO: replace with channel in PM2b

	}

}

/// @}


