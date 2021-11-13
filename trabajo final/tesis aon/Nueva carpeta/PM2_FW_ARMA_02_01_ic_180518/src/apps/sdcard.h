// $Header
/// @file			sdcard.h
/// @brief  		SD Card functions.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 December 2016
/// @details
/// @copyright
///

#ifndef _SD_CARD_H_
#define _SD_CARD_H_

/**************************** INCLUDES ***************************************/

#include "settings.h"

#include "../apis/dsp_api.h"

/**************************** MACROS *****************************************/

#define STRING_MAX		      (128)
#define SDCARD_MEMORY_MAX_MB  (8*1024) // 8GB

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

#define SDCARD_FLAGS_STATUS_ENABLE_SDCARD          ((int8_t)(1u<<0))
#define SDCARD_FLAGS_STATUS_DETECT_SDCARD          ((int8_t)(1u<<1))
#define SDCARD_FLAGS_STATUS_LOGIC_MOUNT            ((int8_t)(1u<<2))
#define SDCARD_FLAGS_STATUS_READ_UNIT_CONFIG       ((int8_t)(1u<<3))
#define SDCARD_FLAGS_STATUS_READ_UNIT_CALIBRATION  ((int8_t)(1u<<4))
#define SDCARD_FLAGS_STATUS_CARD_READY             ((int8_t)(1u<<5))
typedef uint8_t sdcard_flags_status_t;

#define SDCARD_FLAGS_TASK_SAVE_TRENDS_150CYCLES    ((int8_t)(1u<<0))
#define SDCARD_FLAGS_TASK_SAVE_TRENDS_5MINUTES     ((int8_t)(1u<<1))
#define SDCARD_FLAGS_TASK_SAVE_EVENT_DET_INFO      ((int8_t)(1u<<2))
#define SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER_INFO   ((int8_t)(1u<<3))
#define SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER        ((int8_t)(1u<<4))
typedef uint8_t sdcard_flags_task_t;

/**************************** VARIABLES **************************************/

// flags
extern volatile sdcard_flags_status_t sdcard_flags_status;
extern volatile sdcard_flags_task_t   sdcard_flags_task;

// temporal
extern volatile trend_t      sdcard_trend_150cycles;
extern volatile trend_t      sdcard_trend_5minutes;
extern volatile event_det_t  sdcard_event_det;
extern volatile uint16_t     sdcard_event_buffer_info[3];
extern volatile int16_t      sdcard_event_buffer[SETTINGS_EVENT_BUFFER_NUM];

/**************************** FUNCTIONS **************************************/

/// @cond

void sdcard_background(void);

bool sdcard_save_unit_config(void);
bool sdcard_save_unit_calibration(void);
bool sdcard_save_flags(uint16_t flags);

uint32_t sdcard_get_total_mbytes_req(void);
uint32_t sdcard_get_free_mbytes_req(void);

uint8_t sdcard_update_file_and_dir_to_remove(uint8_t * str_name );

uint8_t sdcard_file_get_name(DIR * directory, FILINFO * file_info , uint8_t * name_string , uint8_t limit);

/// @endcond

#endif  // end of _SD_CARD_H_ definition

//#################################### FIN #################################
















  




















