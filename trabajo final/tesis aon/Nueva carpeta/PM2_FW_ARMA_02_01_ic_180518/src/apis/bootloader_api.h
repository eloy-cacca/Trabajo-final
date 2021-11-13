// $Header
/// @file			bootloader_api.h
/// @brief  		Bootloadr application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		20 December 2017
/// @details
/// @copyright
///

#ifndef __BOOTLOADER_API_H__
#define __BOOTLOADER_API_H__

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

void API_BOOT_ARM_start_upload_req( msg_t * msg_ptr );
void API_BOOT_ARM_upload_block_req( msg_t * msg_ptr );
void API_BOOT_ARM_install_image_req( msg_t * msg_ptr );

/// @endcond

#endif // __BOOTLOADER_API_H__

