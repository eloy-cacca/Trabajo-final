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

#include "sdcard.h"
#include "../utils/iqmath.h"
#include "../utils/byte_string_operations.h"
#include "../utils/calculus.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

volatile unit_config_t unit_config;
volatile unit_calibration_t unit_calibration;
volatile comm_flags_t comm_flags;
volatile uint8_t DSP_fw[2] = {0,0};
uint32_t ARM_unique_id[4];

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
	
}

void unit_config_set_custom(unit_config_t * unit_config_new) {

	// copy new configuration
	copy_content( (uint8_t *)unit_config_new ,
	              (uint8_t *)&unit_config ,
	              sizeof(unit_config_t) );

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

}


void unit_calibration_set_custom(unit_calibration_t * unit_calibration_new) {

	// copy new configuration
	copy_content( (uint8_t *)unit_calibration_new ,
	              (uint8_t *)&unit_calibration ,
                  sizeof(unit_calibration_t) );

}

/*****************************************************************************/

void unit_config_and_calibration_init(void) {

	// clear variables
	memset( (void *)&unit_config         , 0 , sizeof(unit_config_t)         );
	memset( (void *)&unit_calibration    , 0 , sizeof(unit_calibration_t)    );
	memset( (void *)&sdcard_flags_status , 0 , sizeof(sdcard_flags_status_t) );
	memset( (void *)&comm_flags          , 0 , sizeof(comm_flags_t)          );

	// Read unique ID
	{
		uint32_t error;
		do {
			error = flash_read_unique_id(ARM_unique_id , 4);
		} while (error);
	}
		

	// set unit config to default (optional)
	unit_config_set_default();
	unit_calibration_set_default();

	// set detection of SD-Card and read unit configuration
#if SETTINGS_ARM_ENABLE_SDCARD_FROM_BEGINNING
	sdcard_flags_status = SDCARD_FLAGS_STATUS_ENABLE_SDCARD;
#endif

	// set DSP communication flags. Request DSP FW version always
	comm_flags = SETTINGS_COMM_FLAG_FW_REQUEST_MASK;

}

/// @}

