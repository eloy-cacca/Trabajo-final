// $Header
/// @file			DSP2833x_library.h
/// @brief  		DSP2833x Library.
/// @details
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		15 Dicember 2014
/// @details
/// @copyright
///

#ifndef __DSP2833X_LIBRARY_H__
#define __DSP2833X_LIBRARY_H__

/**************************** INCLUDES ***************************************/

#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "common_include/DSP2833x_Examples_MOD.h"
#include "common_include/DSP2833x_EPwm_defines.h"

#include "header_include/DSP2833x_Device.h"

typedef int16         int8_t;  // F28335 do not allow the use of 8 bit variables
typedef Uint16        uint8_t; // F28335 do not allow the use of 8 bit variables
typedef int16         int16_t;
typedef int32         int32_t;
typedef int64         int64_t;
typedef Uint16        uint16_t;
typedef Uint32        uint32_t;
typedef Uint64        uint64_t;
typedef float32       float32_t;
typedef float64       float64_t;

#include "drivers/DSP2833x_drivers_adc.h"
#include "drivers/DSP2833x_drivers_flash.h"
#include "drivers/DSP2833x_drivers_gpio.h"
#include "drivers/DSP2833x_drivers_initDefault.h"
#include "drivers/DSP2833x_drivers_sci.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

/// @endcond

#endif // __DSP2833X_LIBRARY_H__

