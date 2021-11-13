// $Header
/// @file			vspt.h
/// @brief  		Variable Sampling Period Technique.
/// @details
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		12 April 2017
/// @details
/// @copyright
///

#ifndef __VSPT_H__
#define __VSPT_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"

#include "../apps_dsp/signal_processing.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

typedef struct {

	int32_t N_low_Q5;
	int16_t N;

	int32_t error_0_Q15;
	int32_t error_1_Q15;
	int32_t error_2_Q15;

} vspt_t;

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void vspt(vspt_t *vspt_s , sp_t * data);

/// @endcond

#endif // __VSPT_H__

