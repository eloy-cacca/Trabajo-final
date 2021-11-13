// $Header
/// @file			unit_config.h
/// @brief  		Unit Configuration module.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 December 2016
/// @details
/// @copyright
///

#ifndef __UNIT_CONFIG_H__
#define __UNIT_CONFIG_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/************************* TYPEDEF (SD-Card flags) ***************************/



/*************************** TYPEDEF (comm flags) ****************************/

/**************************** VARIABLES **************************************/

extern volatile unit_config_t unit_config;				// unit configuration
extern volatile unit_calibration_t unit_calibration;	// unit configuration
extern volatile comm_flags_t comm_flags;				// Pending DSP communication flags
extern volatile uint8_t DSP_fw[2];						// DSP firmware version
extern uint32_t ARM_unique_id[4];

/**************************** FUNCTIONS **************************************/

/// @cond

void unit_config_set_default(void);
void unit_config_set_custom(unit_config_t * unit_config_new);

void unit_calibration_set_default(void);
void unit_calibration_set_custom(unit_calibration_t * unit_calibration_new);

void unit_config_and_calibration_init(void);

/// @endcond

#endif // __UNIT_CONFIG_H__

