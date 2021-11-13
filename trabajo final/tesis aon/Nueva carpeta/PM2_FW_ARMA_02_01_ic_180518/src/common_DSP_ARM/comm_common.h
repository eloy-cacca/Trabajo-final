// $Header
/// @file			comm_common.h
/// @brief 			Settings for DSP and ARM.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 March 2017
/// @details
/// @copyright
///

#ifndef __COMM_COMMON_H__
#define __COMM_COMMON_H__

//****************************************************************************
// UART Baud rate
//****************************************************************************
//#define SETTINGS_COMM_DSP_BAUDRATE        ( 1562500 ) // closest to 1.5MHz
//#define SETTINGS_COMM_ARM_BAUDRATE        ( 1500000 )
#define SETTINGS_COMM_DSP_BAUDRATE          (  937500 ) // closest to 1.0MHz
#define SETTINGS_COMM_ARM_BAUDRATE          (  937500 )

//*****************************************************************************
// Communication protocol
//*****************************************************************************
#define SETTINGS_MSG_LENGHT_HEADER   		( 8 )   // 1 Start + 1 Proj. ID + 2 Dev. ID + 1 Msg. Id + 1 payload length + 2 CRC
#define SETTINGS_MSG_LENGHT_PAYLOAD  		( 128 )
#define SETTINGS_MSG_LENGHT_MAX_SIZE 		( SETTINGS_MSG_LENGHT_HEADER + SETTINGS_MSG_LENGHT_PAYLOAD )
#define SETTINGS_MSG_START_BYTE      		((uint8_t)(0x3C))

//*****************************************************************************
// Communication flags. USED FOR PENDING COMMUNICATIONS BETWEEN ARM AND DSP
//*****************************************************************************
#define SETTINGS_COMM_FLAG_FW_REQUEST_MASK                ((int16_t)(1u<<0))
#define SETTINGS_COMM_FLAG_REQUEST_MASK                   ((int16_t)(1u<<1)) // set only by ARM
#define SETTINGS_COMM_FLAG_RTC_REQUEST_MASK               ((int16_t)(1u<<2))
#define SETTINGS_COMM_FLAG_UNIT_CONFIG_REQUEST_MASK       ((int16_t)(1u<<3))
#define SETTINGS_COMM_FLAG_UNIT_CALIBRATION_REQUEST_MASK  ((int16_t)(1u<<4))
#define SETTINGS_COMM_FLAG_TREND_150CYCLE_MASK            ((int16_t)(1u<<5))
#define SETTINGS_COMM_FLAG_TREND_5MINUTES_MASK            ((int16_t)(1u<<6))
#define SETTINGS_COMM_FLAG_TELEMETRY_MASK                 ((int16_t)(1u<<7))
#define SETTINGS_COMM_FLAG_EVENT_BUFFER_MASK              ((int16_t)(1u<<8))
#define SETTINGS_COMM_FLAG_EVENT_DET_MASK                 ((int16_t)(1u<<9))
#if SETTINGS_ARM_ENABLE_SDCARD_COMM_FLAG_SAVE
#define SETTINGS_COMM_FLAG_TEST01                         ((int16_t)(1u<<13))
#define SETTINGS_COMM_FLAG_TEST02                         ((int16_t)(1u<<14))
#endif

typedef uint16_t comm_flags_t;

#endif // __EVENT_COMMON_H__
