	// $Header
/// @file			unit_config.h
/// @brief  		Unit Configuration module.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		09 March 2016
/// @details
/// @copyright
///

/// @addtogroup modules
/// @{

/**************************** INCLUDES ***************************************/

#include "unit_config.h"
#include "../apps_dsp/event.h"
#include "../apps/unit_config.h"

#include "../utils/iqmath.h"
#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

volatile unit_config_t unit_config;
volatile unit_calibration_t unit_calibration;
volatile system_flags_t system_flags;

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

void unit_config_set_default(void) {

	// General
	unit_config.mode_flags = DEFAULT_MODE_FLAGS;

	// Event flags
	unit_config.event_flags = DEFAULT_EVENT_FLAGS;

	// Sampling period set point
	unit_config.Ts_ticks_Q5_SP = DEFAULT_TS_TICKS_Q5_SP;

	// Sampling period controller
	unit_config.Controller_G0_Q10 = DEFAULT_CONTROLLER_G0_Q10;
	unit_config.Controller_G1_Q10 = DEFAULT_CONTROLLER_G1_Q10;
	unit_config.Controller_G2_Q10 = DEFAULT_CONTROLLER_G2_Q10;

	// Sampling period limits
	unit_config.DTs_ticks_Q5_MAX = DEFAULT_DTS_TICKS_Q5_MAX;
	unit_config.DTs_ticks_Q5_MIN = DEFAULT_DTS_TICKS_Q5_MIN;

	// channel gain
	unit_config.gain_volt = DEFAULT_GAIN_VOLT;
	unit_config.gain_curr_A = DEFAULT_GAIN_CURR_A;
	unit_config.gain_curr_B = DEFAULT_GAIN_CURR_B;

	// sd-card memory
	unit_config.sdcard_memory_max_MB = DEFAULT_SDCARD_MEMORY_MB;

	// Event detection and snapshot
	unit_config.event_RMS2_over_voltage_threshold  = DEFAULT_EVENT_RMS_OV_THR;
	unit_config.event_RMS2_under_voltage_threshold = DEFAULT_EVENT_RMS_UV_THR;
	unit_config.event_RMS2_over_current_threshold  = DEFAULT_EVENT_RMS_OC_THR;
	unit_config.event_FREQ_over_freq_threshold = DEFAULT_EVENT_RMS_OF_THR;
	unit_config.event_FREQ_under_freq_threshold = DEFAULT_EVENT_RMS_UF_THR;

	// set state
	CLEAR_FLAG( system_flags.state_dsp , SETTINGS_STATE_DSP_CONFIGURED );

}


void unit_config_set_custom(unit_config_t * unit_config_new) {

	// copy new configuration
	copy_content( (uint8_t *)unit_config_new ,
			      (uint8_t *)&unit_config ,
				  sizeof(unit_config_t) );

	// set state
	SET_FLAG( system_flags.state_dsp , SETTINGS_STATE_DSP_CONFIGURED );



}

/*****************************************************************************/

void unit_calibration_set_default(void) {

	// ADC
	{
		uint16_t index = 0;
		for ( index=0 ; index<CH_ADC_GRID_TOTAL_NUM ; index++ ) {
			if (index<4)
				unit_calibration.adc_ch_gain_Q15[index] = -DEFAULT_GAIN;
			else
				unit_calibration.adc_ch_gain_Q15[index] = DEFAULT_GAIN;
			unit_calibration.adc_ch_offset[index] = DEFAULT_OFFSET;
		}
	}

	// set state
	CLEAR_FLAG( system_flags.state_dsp , SETTINGS_STATE_DSP_CALIBRATED );

}


void unit_calibration_set_custom(unit_calibration_t * unit_calibration_new) {

	// copy new configuration
	copy_content( (uint8_t *)unit_calibration_new ,
	              (uint8_t *)&unit_calibration ,
                  sizeof(unit_calibration_t) );

	// set state
	SET_FLAG( system_flags.state_dsp , SETTINGS_STATE_DSP_CALIBRATED );

}


/*****************************************************************************/

void unit_config_and_calibration_init(void) {

	memset( (void *)&system_flags  , 0 , sizeof(system_flags_t) );
    unit_config_set_default();
    unit_calibration_set_default();

#if SETTINGS_REQUEST_CONFIGURATION_FROM_ARM
    system_flags.comm_dsp = SETTINGS_COMM_RPI_DSP_REQUEST_FW | // force ARM to request DSP FW version
    		                SETTINGS_COMM_RPI_DSP_UPDATE_RTC |
    		                SETTINGS_COMM_RPI_DSP_UPDATE_UCONFIG |
    		                SETTINGS_COMM_RPI_DSP_UPDATE_UCALIBRATION;
#endif

}

/// @}

