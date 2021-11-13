// $Header
/// @file			sdcard_api.h
/// @brief  		Firmware application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		02 June 2017
/// @details
/// @copyright
///

#ifndef __SDCARD_API_H__
#define __SDCARD_API_H__

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

void API_SDCARD_dir_open_req( msg_t * msg_ptr );
void API_SDCARD_item_next_req( msg_t * msg_ptr );
void API_SDCARD_item_name_req( msg_t * msg_ptr );
void API_SDCARD_file_size_req( msg_t * msg_ptr );
void API_SDCARD_file_open_req( msg_t * msg_ptr );
void API_SDCARD_file_read_req( msg_t * msg_ptr );
void API_SDCARD_file_close_req( msg_t * msg_ptr );
void API_SDCARD_file_and_dir_remove_req( msg_t * msg_ptr );
void API_SDCARD_get_mbytes_req( msg_t * msg_ptr );


/// @endcond

#endif // __SDCARD_API_H__

