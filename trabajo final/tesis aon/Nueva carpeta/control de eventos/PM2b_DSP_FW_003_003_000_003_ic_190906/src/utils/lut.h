// $Header
/// @file			lut.h
/// @brief 			Look up table.
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		31 October 2016
/// @details        This file implement sine and cosine function by look-up
///                 tables method. Three tables are stored, one with 4000
///                 samples, other with 200 samples and other with 128 samples.
/// @copyright
///

#ifndef __LUT_H__
#define __LUT_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"

//*****************************************************************************
// The following functions are located in "ramfuncs", so they always run from
// RAM and must be relocated when the flash initialization is executed. */
//*****************************************************************************
//#pragma CODE_SECTION(lut_sin_128_Q20, "ramfuncs");
//#pragma CODE_SECTION(lut_cos_128_Q20, "ramfuncs");

/**************************** MACROS *****************************************/

#if 0
#define LUT_SIN_4000_N     (4000)
#define LUT_SIN_200_N      (200)
#endif
#define LUT_SIN_128_N      (128)

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

#if 0
int32_t lut_sin_4000_Q20( int32_t phase );
int32_t lut_cos_4000_Q20( int32_t phase );

int32_t lut_sin_200_Q20( int32_t phase );
int32_t lut_cos_200_Q20( int32_t phase );
#endif
int32_t lut_sin_128_Q20( int32_t phase );
int32_t lut_cos_128_Q20( int32_t phase );

/// @endcond

#endif // __LUT_H__
