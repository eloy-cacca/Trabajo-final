// $Header
/// @file			app_layer.h
/// @brief  		Communication application layer.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		27 December 2016
/// @details
/// @copyright
///

#ifndef APP_LAYER_H_
#define APP_LAYER_H_

/**************************** INCLUDES ***************************************/

// General includes
#include "settings.h"

// Local includes
#include "comm.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void decode_incoming_comms_packet( msg_t * msg_ptr );

/// @endcond

#endif /* APP_LAYER_H_ */
