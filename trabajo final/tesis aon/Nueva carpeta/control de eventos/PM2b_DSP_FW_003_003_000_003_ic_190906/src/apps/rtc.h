// $Header
/// @file			rtc.h
/// @brief  		Real Time Clock file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		12 February 2015
/// @details
/// @copyright
///

#ifndef __RTC_H__
#define __RTC_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

// Real Time clock variable Definition
typedef struct {
	uint16_t m_second;	  // 14bits (0..999)
	uint16_t second;	  // 6bits  (0..59)
	uint16_t minute;	  // 6bits  (0..59)
	uint16_t hour;		  // 5bits  (0..23)
} rtc_t;

// Real time clock flag
#define RTC_FLAG_MSECOND  ((((uint16_t)1)<<0))
#define RTC_FLAG_SECOND   ((((uint16_t)1)<<1))
#define RTC_FLAG_MINUTE   ((((uint16_t)1)<<2))
#define RTC_FLAG_HOUR     ((((uint16_t)1)<<3))

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void rtc_init(void);

void rtc_update(void);
rtc_t * rtc_get_time(void);
void rtc_set_time(rtc_t * rtc_ptr);

uint16_t rtc_alarm5minute_flag(void);
void rtc_alarmSecondCount_start(void);
uint16_t rtc_alarmSecondCount_check(uint32_t * threshold);

uint16_t rtc_get_flags(void);

/*
void rtc_alarm2000msecond_start(void);
bool rtc_alarm2000msecond_flag(void);
void rtc_alarm2000msecond_force_flag(void);
*/

/// @endcond

#endif // __RTC_H__

