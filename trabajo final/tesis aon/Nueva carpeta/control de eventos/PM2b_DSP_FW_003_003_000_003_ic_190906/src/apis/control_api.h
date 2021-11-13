// $Header
/// @file			control_api.h
/// @brief  		Control application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		02 November 2016
/// @details
/// @copyright
///

#ifndef __CONTROL_API_H__
#define __CONTROL_API_H__

/**************************** INCLUDES ***************************************/

// General includes
#include "../settings.h"

// Local includes
#include "../apps_comm/comm.h"
#include "../apps_dsp/trend.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void API_CTRL_trend_150cycles_req( msg_t * msg_ptr );
void API_CTRL_trend_5minutes_req( msg_t * msg_ptr );
void API_CTRL_event_buffer_req( msg_t * msg_ptr );
void API_CTRL_event_det_req( msg_t * msg_ptr );
void API_CTRL_telemetria_req( msg_t * msg_ptr );

/// @endcond

#endif // __CONTROL_API_H__

