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

/**************************** MACROS *****************************************/
#define LUT_128_N               (128)


/**************************** FUNCTIONS **************************************/
int32_t lut_sin_128_Q20(uint8_t phase);
int32_t lut_cos_128_Q20(uint8_t phase);
int32_t lut_custom_128_Q20(uint8_t phase);
void set_custom_lut(int32_t * new_table);


#endif // __LUT_H__
