// $Header
/// @file			unit_config_api.h
/// @brief  		Unit config application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		12 August 2016
/// @details
/// @copyright
///

#ifndef __UNIT_CONFIG_API_H__
#define __UNIT_CONFIG_API_H__

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

void API_UNIT_CONFIG_DSP_update_req( msg_t * msg_ptr );
void API_UNIT_CONFIG_DSP_send_req( msg_t * msg_ptr );
void API_UNIT_CONFIG_DSP_set_default_req( msg_t * msg_ptr );
void API_UNIT_CALIBRATION_DSP_update_req( msg_t * msg_ptr );
void API_UNIT_CALIBRATION_DSP_send_req( msg_t * msg_ptr );
void API_UNIT_CALIBRATION_DSP_set_default_req( msg_t * msg_ptr );

//void API_UC_unit_config_update_req( void);
//void API_UC_unit_config_update_res( msg_t * msg_ptr );
//void API_UC_unit_config_force_update( msg_t * msg_ptr );


/// @endcond

#endif // __UNIT_CONFIG_API_H__

