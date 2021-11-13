// $Header
/// @file			unit_config_api.h
/// @brief  		Unit config application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		28 December 2016
/// @details
/// @copyright
///

#ifndef __UNIT_CONFIG_API_H__
#define __UNIT_CONFIG_API_H__

/**************************** INCLUDES ***************************************/

// General includes
#include "../settings.h"

// Local includes
#include "../comm/comm.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void API_UNIT_CONFIG_ARM_update_req( msg_t * msg_ptr );
void API_UNIT_CONFIG_ARM_send_req( msg_t * msg_ptr );
void API_UNIT_CONFIG_ARM_set_default_req( msg_t * msg_ptr );

void API_UNIT_CALIBRATION_ARM_update_req( msg_t * msg_ptr );
void API_UNIT_CALIBRATION_ARM_send_req( msg_t * msg_ptr );
void API_UNIT_CALIBRATION_ARM_set_default_req( msg_t * msg_ptr );

/// @endcond

#endif // __UNIT_CONFIG_API_H__

