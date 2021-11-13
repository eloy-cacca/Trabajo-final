// $Header
/// @file			settings.h
/// @brief 			Project settings.
/// @details
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		16 October 2014
/// @details
/// @copyright
///

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "DSP2833x_library.h"

//*****************************************************************************
// DSP - COMPILER OPTIONS
//*****************************************************************************
#define SETTINGS_REQUEST_CONFIGURATION_FROM_ARM   (1)
#define SETTINGS_ISR_TYPE						  (0) // (1) TIMER0 or (0) PWM/ADC
#define SETTINGS_ARM_ENABLE_SDCARD_COMM_FLAG_SAVE (0)

//*****************************************************************************
// Firmware identifications
//*****************************************************************************
#define SETTINGS_DSP_FW_VERSION					(0)
#define SETTINGS_DSP_FW_VERSION_ENG_BUILD		(3)
#define SETTINGS_DSP_FW_AUTHOR              	'IC'

//*****************************************************************************
// ARM and DSP common files
//*****************************************************************************
#include "common_DSP_ARM/settings_common.h"
#include "common_DSP_ARM/unit_config_common.h"
#include "common_DSP_ARM/trend_common.h"
#include "common_DSP_ARM/event_common.h"
#include "common_DSP_ARM/app_layer_common.h"
#include "common_DSP_ARM/comm_common.h"

#endif // __SETTINGS_H__
