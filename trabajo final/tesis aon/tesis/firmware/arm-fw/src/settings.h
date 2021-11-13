/**
 * \file           settings.h
 * \brief          Settings file.
 * \details
 * \author         Agustin Aon
 * \version        0.1
 * \date           20 December 2019
 * \details
 * \copyright
 *
 * \addtogroup settings
 * \{
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <asf.h>
#include <string.h>

//*****************************************************************************
// Device identifications
//*****************************************************************************
#define SETTINGS_PROJECT_NAME               "Aon Meter V0.1"
#define SETTINGS_DEVICE_NAME                "AM0.1"
#define SETTINGS_PROJECT_ID                 (0x04)
#define SETTINGS_ARM_ID                     (0x77)
#define SETTINGS_PC_ID                      (0xAA)
#define SETTINGS_OTHERS_ID                  (0xEE)

//*****************************************************************************
// ARM - Firmware identifications
//*****************************************************************************
#define SETTINGS_ARM_FW_VERSION             (1)
#define SETTINGS_ARM_FW_VERSION_ENG_BUILD   (2)
#define SETTINGS_ARM_FW_AUTHOR              'AA'

//*****************************************************************************
// ARM - Clock Timer Configuration
//*****************************************************************************
#define SETTINGS_ARM_TIMER_FREQ_HZ          (6400)          // Fs

//****************************************************************************
// UART Baud rate
//****************************************************************************
#define SETTINGS_COMM_ARM_BAUDRATE          (937500)

//*****************************************************************************
// ARM - Control Configuration
//*****************************************************************************
#define SETTINGS_INDEX_ACCUM_MAX            (1280)          // 10 cycles x 128 samples/cycle
#define SETTINGS_TREND_MAX                  (15)            // 3 segundos = fs/SETTINGS_ACCUM_MAX
#define SETTINGS_NDFT_MAX                   (4)             // Max harmonics to measure (without DC)
#define SETTINGS_CH_TOTAL_NUM               (4)
#define SETTINGS_MSDFT_BUF_LEN              (128)

//*****************************************************************************
// ARM - PWM
//*****************************************************************************
#define SETTINGS_PWM_BUF_LEN                (2)             // 1 for each channel

//*****************************************************************************
// System flags
//*****************************************************************************
// Trends
#define SETTINGS_TRENDS_FREQ_CHANGED                (1 << 0)
#define SETTINGS_TRENDS_AMP_CHANGED                 (1 << 1)
#define SETTINGS_TRENDS_ENABLED_CHANGED             (1 << 2)
#define SETTINGS_TRENDS_CUSTOM_WAVEFORM_CHANGED     (1 << 3)


#endif /* SETTINGS_H_ */