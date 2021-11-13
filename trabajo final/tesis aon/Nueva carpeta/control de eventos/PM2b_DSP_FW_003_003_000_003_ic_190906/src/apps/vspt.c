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

/// @addtogroup Modules
/// @{

/**************************** INCLUDES ***************************************/

#include "vspt.h"
#include "unit_config.h"
#include "system.h"
#include "../utils/iqmath.h"
#include "../utils/lut.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

void vspt(vspt_t * vspt_s , sp_t * data) {

	// Fixes mode is enable?
	if (unit_config.mode_flags & UNIT_CONFIG_MODE_TS_FSPT) {

		vspt_s->N = (unit_config.Ts_ticks_Q5_SP)>>5;

	}
	else {

		// compute error and save data for next instant
		vspt_s->error_2_Q15 = vspt_s->error_1_Q15;
		vspt_s->error_1_Q15 = vspt_s->error_0_Q15;
		//vspt_s->error_0_Q15 = FPDIV( data->v_seq_pos.imag , data->v_seq_pos.real , 15 );
		vspt_s->error_0_Q15 = - FPDIV( data->ch[0].harmonic[1].imag , ABS(data->ch[0].harmonic[1].real)>>15 , 0 );

		// Controller for small deviation from nominal frequency
		vspt_s->N_low_Q5 += FPMUL( unit_config.Controller_G0_Q10 , vspt_s->error_0_Q15 , 20 );
		vspt_s->N_low_Q5 += FPMUL( unit_config.Controller_G1_Q10 , vspt_s->error_1_Q15 , 20 );
		vspt_s->N_low_Q5 += FPMUL( unit_config.Controller_G2_Q10 , vspt_s->error_2_Q15 , 20 );

		// Limiter
		if 		( vspt_s->N_low_Q5 > unit_config.DTs_ticks_Q5_MAX) {
			vspt_s->N_low_Q5 = unit_config.DTs_ticks_Q5_MAX;
			vspt_s->error_0_Q15 = vspt_s->error_1_Q15; // avoid divergence of error
		}
		else if ( vspt_s->N_low_Q5 < unit_config.DTs_ticks_Q5_MIN){
			vspt_s->N_low_Q5 = unit_config.DTs_ticks_Q5_MIN;
			vspt_s->error_0_Q15 = vspt_s->error_1_Q15; // avoid divergence of error
		}

		// Compute current sampling period in ticks
		vspt_s->N = (vspt_s->N_low_Q5 + unit_config.Ts_ticks_Q5_SP)>>5;

	}

	// set TIMER0 period
#if SETTINGS_ISR_TYPE
	cputimer0_set_period = vspt_s->N;
#else
	epwm1_set_period = vspt_s->N;
#endif

	// Return from function
	return;

}

/// @}

