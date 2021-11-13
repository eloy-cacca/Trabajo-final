// $Header
/// @file			dsp_api.h
/// @brief  		DSP application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		09 February 2017
/// @details
/// @copyright
///

#ifndef __DSP_API_H__
#define __DSP_API_H__

/**************************** INCLUDES ***************************************/

// General includes
#include "settings.h"

// Local includes
#include "../comm/comm.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void API_FW_DSP_version_req(void);
void API_FW_DSP_version_res(msg_t * msg_ptr);

void API_FW_DSP_send_comm_flag_req(void);
void API_FW_DSP_send_comm_flag_res(msg_t * msg_ptr);

void API_RTC_DSP_update_req(void);
void API_RTC_DSP_update_res(msg_t * msg_ptr);

void API_UNIT_CONFIG_DSP_update_req(void);
void API_UNIT_CONFIG_DSP_update_res(msg_t * msg_ptr);

void API_UNIT_CALIBRATION_DSP_update_req(void);
void API_UNIT_CALIBRATION_DSP_update_res(msg_t * msg_ptr);

void API_CTRL_DSP_trend_150cycles_req(void);
void API_CTRL_DSP_trend_150cycles_res(msg_t * msg_ptr);

void API_CTRL_DSP_trend_5minutes_req(void);
void API_CTRL_DSP_trend_5minutes_res(msg_t * msg_ptr);

void API_CTRL_DSP_event_buffer_req(void);
void API_CTRL_DSP_event_buffer_res(msg_t * msg_ptr);

void API_CTRL_DSP_event_det_req(void);
void API_CTRL_DSP_event_det_res(msg_t * msg_ptr);


/// @endcond

#endif // __DSP_API_H__

