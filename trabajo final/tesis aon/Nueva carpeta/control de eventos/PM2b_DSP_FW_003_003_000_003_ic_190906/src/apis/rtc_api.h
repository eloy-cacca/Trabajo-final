// $Header
/// @file			rtc_api.h
/// @brief  		Real Time Clock application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		24 Nobember 2016
/// @details
/// @copyright
///

#ifndef __RTC_API_H__
#define __RTC_API_H__

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

void API_RTC_DSP_update_req( msg_t * msg_ptr );
void API_RTC_DSP_send_req( msg_t * msg_ptr );

//void API_RTC_date_update_req(void);

/// @endcond

#endif // __RTC_API_H__

