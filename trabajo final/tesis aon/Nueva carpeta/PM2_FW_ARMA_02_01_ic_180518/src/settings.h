// $Header
/// @file			settings.h
/// @brief 			Project settings.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 February 2017
/// @details
/// @copyright
///

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "asf.h"
#include <string.h>

//*****************************************************************************
// ARM - COMPILER OPTIONS
//*****************************************************************************
#define SETTINGS_ARM_ENABLE_SDCARD_FROM_BEGINNING       (1)
#define SETTINGS_ARM_ENABLE_SDCARD_COMM_FLAG_SAVE       (1)

//*****************************************************************************
// ARM - Firmware identifications
//*****************************************************************************
#define SETTINGS_ARM_FW_VERSION							(2)
#define SETTINGS_ARM_FW_VERSION_ENG_BUILD				(3)
#define SETTINGS_ARM_FW_AUTHOR							'IC'

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
