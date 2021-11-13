// $Header
/// @file			control.h
/// @brief  		Control and processing module.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		29 February 2016
/// @details
/// @copyright
///

#ifndef __CONTROL_H__
#define __CONTROL_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"
#include "../apps_dsp/trend.h"
#include "../apps_dsp/signal_processing.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void control_update(void);
void control_init(void);
void control_start(void);
void control_background(void);

trend_t * ctrl_ptr_to_trend_150cycles(void);
trend_t * ctrl_ptr_to_trend_5m(void);
event_buffer_t * ctrl_ptr_to_event_buffer(void);
event_det_t * ctrl_ptr_to_event_det(void);
uint16_t * ctrl_trend_flag_temp_ptr(void);

/// @endcond

#endif // __CONTROL_H__




  




















