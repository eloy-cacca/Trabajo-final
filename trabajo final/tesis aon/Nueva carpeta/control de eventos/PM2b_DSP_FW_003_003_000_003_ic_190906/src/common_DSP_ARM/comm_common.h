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
// System flags.
//*****************************************************************************
// Communication . USED FOR PENDING COMMUNICATIONS BETWEEN RPI AND DSP
#define SETTINGS_COMM_RPI_DSP_REQUEST_FW             ((int16_t)(1u<<0))
#define SETTINGS_COMM_RPI_DSP_REQUEST_COMM           ((int16_t)(1u<<1))
#define SETTINGS_COMM_RPI_DSP_UPDATE_RTC             ((int16_t)(1u<<2))
#define SETTINGS_COMM_RPI_DSP_UPDATE_UCONFIG         ((int16_t)(1u<<3))
#define SETTINGS_COMM_RPI_DSP_UPDATE_UCALIBRATION    ((int16_t)(1u<<4))
#define SETTINGS_COMM_RPI_DSP_REQUEST_TREND_3S       ((int16_t)(1u<<5))
#define SETTINGS_COMM_RPI_DSP_REQUEST_TREND_5M       ((int16_t)(1u<<6))
#define SETTINGS_COMM_RPI_DSP_REQUEST_EVENT_DET      ((int16_t)(1u<<7))
#define SETTINGS_COMM_RPI_DSP_REQUEST_EVENT_BUFFER   ((int16_t)(1u<<8))

// Communication . USED FOR PENDING COMMUNICATIONS BETWEEN RPI AND ARM
#define SETTINGS_COMM_RPI_ARM_REQUEST_FW             ((int16_t)(1u<<0))
#define SETTINGS_COMM_RPI_ARM_REQUEST_COMM           ((int16_t)(1u<<1))
#define SETTINGS_COMM_RPI_ARM_REQUEST_RTC            ((int16_t)(1u<<2))
#define SETTINGS_COMM_RPI_ARM_UPDATE_RTC             ((int16_t)(1u<<3))
#define SETTINGS_COMM_RPI_ARM_REQUEST_ADC            ((int16_t)(1u<<4))
#define SETTINGS_COMM_RPI_ARM_ARMBOOT_START_UPLOAD   ((int16_t)(1u<<5))
#define SETTINGS_COMM_RPI_ARM_ARMBOOT_UPDATE_BLOCK   ((int16_t)(1u<<6))
#define SETTINGS_COMM_RPI_ARM_ARMBOOT_INSTALL_IMAGE  ((int16_t)(1u<<7))
#define SETTINGS_COMM_RPI_ARM_DSPBOOT_START_UPLOAD   ((int16_t)(1u<<8))
#define SETTINGS_COMM_RPI_ARM_DSPBOOT_UPDATE_BLOCK   ((int16_t)(1u<<9))
#define SETTINGS_COMM_RPI_ARM_DSPBOOT_INSTALL_IMAGE  ((int16_t)(1u<<10))

// State RPI
#define SETTINGS_STATE_RPI_FWS_REQUEST               ((int16_t)(1u<<0))
#define SETTINGS_STATE_RPI_CONFIGURED                ((int16_t)(1u<<1))
#define SETTINGS_STATE_RPI_CALIBRATED                ((int16_t)(1u<<2))
#define SETTINGS_STATE_RPI_RTC_UPDATED               ((int16_t)(1u<<3))
#define SETTINGS_STATE_RPI_PENDING_DATA              ((int16_t)(1u<<4))
// State ARM
#define SETTINGS_STATE_ARM_RTC_UPDATED               ((int16_t)(1u<<0))
#define SETTINGS_STATE_ARM_RTC_FAULT                 ((int16_t)(1u<<1))
#define SETTINGS_STATE_ARM_GRID_PRESENT              ((int16_t)(1u<<2))
#define SETTINGS_STATE_ARM_ENABLE_SAVE_DATA          ((int16_t)(1u<<3))
// State DSP
#define SETTINGS_STATE_DSP_CONFIGURED                ((int16_t)(1u<<0))
#define SETTINGS_STATE_DSP_CALIBRATED                ((int16_t)(1u<<1))
#define SETTINGS_STATE_DSP_RTC_UPDATED               ((int16_t)(1u<<2))
#define SETTINGS_STATE_DSP_PENDING_DATA              ((int16_t)(1u<<3))

typedef struct {
	uint16_t comm_arm;
	uint16_t comm_dsp;
	uint8_t state_rpi;
	uint8_t state_arm;
	uint8_t state_dsp;
} system_flags_t;

#endif // __EVENT_COMMON_H__
