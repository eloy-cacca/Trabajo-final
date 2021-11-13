// $Header
/// @file			meter_ctrl.h
/// @brief  		Meter control application.
/// @details		It define the state of supply module and raspberry pi.
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		30 October 2017
/// @details
/// @copyright
///

#ifndef __METER_CTRL_H__
#define __METER_CTRL_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

// meter control state machine
enum meter_ctrl_state_enum {
	METER_CTRL_STATE_INIT = 0,
	METER_CTRL_STATE_IDLE,
	METER_CTRL_STATE_CONFIRM_LOW_VIN,
	METER_CTRL_STATE_WAIT_RPI_TURNOFF,
	METER_CTRL_STATE_LOW_VIN,
	METER_CTRL_STATE_SHUTDOWN_1,
	METER_CTRL_STATE_SHUTDOWN_2,
	METER_CTRL_STATE_SHUTDOWN_3
};

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void meter_ctrl_init(void);
void meter_ctrl_update(int32_t * vin_Q11);
enum meter_ctrl_state_enum * meter_ctrl_get_state(void);

/// @endcond

#endif // __METER_CTRL_H__

