// $Header
/// @file			fw_api.h
/// @brief  		Firmware application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		28 December 2016
/// @details
/// @copyright
///

#ifndef __FW_API_H__
#define __FW_API_H__

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

void API_FW_ARM_version_req( msg_t * msg_ptr );
void API_FW_ARM_echo_req( msg_t * msg_ptr );
void API_FW_ARM_unique_id_req( msg_t * msg_ptr );
void API_FW_ARM_reset_req( msg_t * msg_ptr );

void API_FW_ARM_unknow_msg_id( msg_t * msg_ptr );

/// @endcond

#endif // __FW_API_H__

