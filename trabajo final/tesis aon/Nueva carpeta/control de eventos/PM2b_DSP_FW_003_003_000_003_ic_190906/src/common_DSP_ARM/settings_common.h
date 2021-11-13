// $Header
/// @file			settings_common.h
/// @brief 			Settings for DSP and ARM.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 February 2017
/// @details
/// @copyright
///

#ifndef __SETTINGS_COMMON_H__
#define __SETTINGS_COMMON_H__

//*****************************************************************************
// Device identifications
//*****************************************************************************
#define SETTINGS_PROJECT_NAME               "Smart Power Quality Meter v2b"
#define SETTINGS_DEVICE_NAME                "LICPM2b"
#define SETTINGS_PROJECT_ID                 (0x03)
#define SETTINGS_RPI_ID                     (0x01)
#define SETTINGS_ARM_ID                     (0x02)
#define SETTINGS_DSP_ID                     (0x03)
#define SETTINGS_OTHERS_ID                  (0x04)

//*****************************************************************************
// ARM - Clock Timer Configuration
//*****************************************************************************
#define SETTINGS_ARM_TIMER_FREQ_HZ          (10000) // Period = 100us

//*****************************************************************************
// DSP - Hardware
//*****************************************************************************
#define SETTINGS_DSP_CPU_MODEL              (28335)                           // TMS320F28335
#define SETTINGS_DSP_CLK_MHZ                (30.0)                            // 30MHz
#define SETTINGS_DSP_SYSCLK_MHZ             (SETTINGS_DSP_CLK_MHZ * 5)        // 150MHz
#define SETTINGS_DSP_SYSCLK_KHZ             (SETTINGS_DSP_SYSCLK_MHZ * 1000)  // 150MHz
#define SETTINGS_DSP_SYSCLK_HZ              (SETTINGS_DSP_SYSCLK_KHZ * 1000)  // 150MHz
#define SETTINGS_DSP_HSPCLK_HZ              (SETTINGS_DSP_CLK_MHZ / 2)        // 75MHz (Default high speed peripheral clk)
#define SETTINGS_DSP_LSPCLK_HZ              (SETTINGS_DSP_CLK_MHZ / 4)        // 37.5MHz (Default low speed peripheral clk)

//*****************************************************************************
// DSP - Sampling period technique
//*****************************************************************************
#define SETTINGS_DSP_NPLL_EXP               (7)   // samples per fundamental cycle (exponent)
#define SETTINGS_DSP_NPLL                   (128) // 2^7=128

//*****************************************************************************
// DSP - Analog to digital converter
//*****************************************************************************
#define SETTINGS_DSP_ADC_CH_NUMBER          (8) // Total number of ADC channels
#define SETTINGS_DSP_ADC_MAX_TICK           (32768.0) // Maximum positive/negative value

//*****************************************************************************
// DSP - Processing
//*****************************************************************************
#define SETTINGS_DSP_NDFT_MAX				(4)    // Maximum amount of harmonics to measure (no included DC)


#endif // __SETTINGS_COMMON_H__
