// $Header
/// @file			event.h
/// @brief  		Event detection file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 February 2017
/// @details
/// @copyright
///

#ifndef __EVENT_H__
#define __EVENT_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"
#include "signal_processing.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void event_buffer_init( event_buffer_t * event );
void event_update( event_buffer_t * event , event_det_t * event_dec , int32_t * data , sp_t * data_processed , uint16_t * counter);
void event_backgound(event_buffer_t * event_buffer , event_det_t * event_det);

/// @endcond

#endif // __EVENT_H__

