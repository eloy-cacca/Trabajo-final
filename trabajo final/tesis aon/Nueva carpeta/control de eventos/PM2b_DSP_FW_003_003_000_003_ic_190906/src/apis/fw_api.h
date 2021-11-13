// $Header
/// @file			fw_api.h
/// @brief  		Firmware application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		05 August 2016
/// @details
/// @copyright
///

#ifndef __FW_API_H__
#define __FW_API_H__

/**************************** INCLUDES ***************************************/

// General includes
#include "../settings.h"

// Local includes
#include "../apps_comm/comm.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void API_FW_version_req( msg_t * msg_ptr );
void API_FW_echo_req( msg_t * msg_ptr );
void API_FW_dsp_send_comm_flag_req( msg_t * msg_ptr );
void API_FW_unknow_msg_id( msg_t * msg_ptr );

//void API_FW_unknow_msg_id( msg_t * msg_ptr );
//void API_FW_unit_start_send( void );

/// @endcond

#endif // __FW_API_H__

