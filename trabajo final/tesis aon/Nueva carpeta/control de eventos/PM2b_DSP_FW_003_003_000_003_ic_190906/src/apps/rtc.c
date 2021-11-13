// $Header
/// @file			rtc.c
/// @brief  		Real Time Clock file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		2.0
/// @date      		24 November 2016
/// @details
/// @copyright
///

/// @addtogroup apps
/// @{

/**************************** INCLUDES ***************************************/

#include "rtc.h"
#include "../utils/byte_string_operations.h"
#include "../apps/unit_config.h"
#include "../apps/system.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

typedef struct {

//	uint16_t count;
	uint16_t flag;

} rtc_alarm_t;


/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

uint32_t DSP_ticks;

rtc_t rtc;

uint16_t alarm5minute_flag;
uint32_t alarmSecondCount;

uint16_t rtc_flags;

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

void rtc_init(void) {

	// init rtc registers
	DSP_ticks = 0;
	rtc.m_second = 0;
	rtc.second = 0;
	rtc.minute = 0;
	rtc.hour = 0;

	// init flags
	rtc_flags = 0;

	// init alarm
	alarm5minute_flag = 0;
	alarmSecondCount = 0;

//	rtc_5minute_alarm.count = 0;

//	rtc_2000mseconds_alarm.count = 0;
//	rtc_2000mseconds_alarm.flag = 0;

	// set state
	//CLEAR_FLAG( system_flags.state , SETTINGS_STATE_DSP_RTC_UPDATED );

}


void rtc_update(void) {

#if SETTINGS_ISR_TYPE
	DSP_ticks += (CpuTimer0Regs.PRD.all + 1);
#else
//	DSP_ticks += (epwm1_set_period);
	DSP_ticks += (epwm1_set_period + 1);
#endif


	rtc_flags = 0;

	if (DSP_ticks> (SETTINGS_DSP_SYSCLK_KHZ-1) ) {
		DSP_ticks -= (SETTINGS_DSP_SYSCLK_KHZ-1);
		rtc.m_second++;
		alarmSecondCount++;
		rtc_flags |= RTC_FLAG_MSECOND;
//		rtc_2000mseconds_alarm.count++;
		if (rtc.m_second>999) {
			rtc.m_second = 0;
			rtc.second++;
			rtc_flags |= RTC_FLAG_SECOND;
			if (rtc.second>59) {
				rtc.second = 0;
				rtc.minute++;
				rtc_flags |= RTC_FLAG_MINUTE;
				if ( ((rtc.minute % 10) == 0) || ((rtc.minute % 5)==0) ) {
#if SETTINGS_ARM_ENABLE_SDCARD_COMM_FLAG_SAVE
#include "../common_DSP_ARM/comm_common.h"
					comm_flags |= SETTINGS_COMM_FLAG_TEST01;
#endif
					alarm5minute_flag = 1;
				}
				//rtc_5minute_alarm.count++;
				if (rtc.minute>59){
					rtc.minute=0;
					rtc.hour++;
					rtc_flags |= RTC_FLAG_HOUR;
					if (rtc.hour>23){
						rtc.hour=0;
#if SETTINGS_REQUEST_CONFIGURATION_FROM_ARM
						system_flags.comm_dsp |= SETTINGS_COMM_RPI_DSP_UPDATE_RTC; // if RTC reach this value, update time
#endif
					}
				}
			}
		}
	}

//	if (rtc_5minute_alarm.count > 4) {
//		rtc_5minute_alarm.flag = 1;
//		rtc_5minute_alarm.count = 0; // start again
//	}

//	if (rtc_2000mseconds_alarm.count > 2000) {
//		rtc_2000mseconds_alarm.flag = 1;
//		rtc_2000mseconds_alarm.count = 2000;// no change
//	}

}

rtc_t * rtc_get_time(void) {

	return &rtc;

}

void rtc_set_time(rtc_t * rtc_ptr) {

	// copy new configuration
	copy_content( (uint8_t *)rtc_ptr ,
			      (uint8_t *)&rtc ,
				  sizeof(rtc_t) );

	// set state
	SET_FLAG( system_flags.state_dsp , SETTINGS_STATE_DSP_RTC_UPDATED );

}


/// @brief Get flag from 5 minute alarm.
/// @note Ones flag is read, it is cleared.
uint16_t rtc_alarm5minute_flag(void) {

	if (alarm5minute_flag == 1) {
		alarm5minute_flag = 0;
		return 1;
	}
	else {
		return 0;
	}

}

/// @brief Start second count of alarm.
void rtc_alarmSecondCount_start(void) {

	alarmSecondCount = 0;
}

/// @brief Check second count of alarm.
uint16_t rtc_alarmSecondCount_check(uint32_t * threshold) {

	if (alarmSecondCount > *threshold)
		return 1;
	else
		return 0;

}

/// @brief Get RTC flags.
uint16_t rtc_get_flags(void) {

	return rtc_flags;

}





/*
/// @brief Get flag from 1000 milliseconds alarm.
/// @note Ones flag is read, it is cleared.
void rtc_alarm2000msecond_start(void) {

	rtc_2000mseconds_alarm.flag = 0;
	rtc_2000mseconds_alarm.count = 0;

}

/// @brief Get flag from 1000 milliseconds alarm.
/// @note Ones flag is read, it is cleared.
bool rtc_alarm2000msecond_flag(void) {

	return rtc_2000mseconds_alarm.flag;

}

/// @brief Force flag from 1000 milliseconds alarm.
void rtc_alarm2000msecond_force_flag(void) {

	rtc_2000mseconds_alarm.count = 2000;

}
*/

/// @}

