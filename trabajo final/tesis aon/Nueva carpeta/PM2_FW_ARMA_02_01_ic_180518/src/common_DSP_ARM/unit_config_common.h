// $Header
/// @file			unit_config_common.h
/// @brief 			Settings for DSP and ARM.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 MARCH 2017
/// @details
/// @copyright
///

#ifndef __UNIT_CONFIG_COMMON_H__
#define __UNIT_CONFIG_COMMON_H__

#include "../utils/calculus.h"

//*****************************************************************************
// channel definition
//*****************************************************************************

// channels for single phase measurement
enum channels_adc_enum{
	CH_ADC_V1_N = 0,
	CH_ADC_V2_N,
	CH_ADC_V3_N,
	CH_ADC_VE_N,
	CH_ADC_I1A,
	CH_ADC_I2A,
	CH_ADC_I3A,
	CH_ADC_IN,
	CH_ADC_I1B,
	CH_ADC_I2B,
	CH_ADC_I3B,
	CH_ADC_IE,
	CH_ADC_VIN_Q11,
	CH_ADC_TOTAL_NUM
	};
#define CH_ADC_GRID_TOTAL_NUM (12)   // number of ADC with currents and voltages from grid

// channels for power measurement
enum channels_power_enum{
	CH_P1A = 0,
	CH_P2A,
	CH_P3A,
	CH_P1B,
	CH_P2B,
	CH_P3B,
	CH_POWER_TOTAL_NUM
	};

//*****************************************************************************
// unit config
//*****************************************************************************

// Mode of operation (32 bits unsigned)
// sampling period
#define UNIT_CONFIG_MODE_TS_POS					 (0)
#define UNIT_CONFIG_MODE_TS_MASK                 (((uint32_t)0xF)<<UNIT_CONFIG_MODE_TS_POS)
#define UNIT_CONFIG_MODE_TS_VSPT                 (((uint32_t)0x0)<<UNIT_CONFIG_MODE_TS_POS)
#define UNIT_CONFIG_MODE_TS_FSPT                 (((uint32_t)0x1)<<UNIT_CONFIG_MODE_TS_POS)
// analog to digital converter
#define UNIT_CONFIG_MODE_ADC_POS				 (4)
#define UNIT_CONFIG_MODE_ADC_MASK                (((uint32_t)0xF)<<UNIT_CONFIG_MODE_ADC_POS)
#define UNIT_CONFIG_MODE_ADC_GRID                (((uint32_t)0x0)<<UNIT_CONFIG_MODE_ADC_POS)
#define UNIT_CONFIG_MODE_ADC_LOW                 (((uint32_t)0x1)<<UNIT_CONFIG_MODE_ADC_POS)
#define UNIT_CONFIG_MODE_ADC_EMUL                (((uint32_t)0x2)<<UNIT_CONFIG_MODE_ADC_POS)
// controller
#define UNIT_CONFIG_MODE_CONTROLLER_POS 		 (8)
#define UNIT_CONFIG_MODE_CONTROLLER_MASK         (((uint32_t)0xF)<<UNIT_CONFIG_MODE_CONTROLLER_POS)
#define UNIT_CONFIG_MODE_CONTROLLER_P            (((uint32_t)0x0)<<UNIT_CONFIG_MODE_CONTROLLER_POS) // y(k) = G0*x(k)
#define UNIT_CONFIG_MODE_CONTROLLER_PI           (((uint32_t)0x1)<<UNIT_CONFIG_MODE_CONTROLLER_POS) // y(k) = G0*x(k) + G1*x(k-1) + y(k-1)
#define UNIT_CONFIG_MODE_CONTROLLER_PID          (((uint32_t)0x2)<<UNIT_CONFIG_MODE_CONTROLLER_POS) // y(k) = G0*x(k) + G1*x(k-1) + G2*x(k-2) + y(k-1)
// enable send of 150 cycles trend to ARM
#define UNIT_CONFIG_MODE_TREND_150CYCLES_POS     (13)
#define UNIT_CONFIG_MODE_TREND_150CYCLES_MASK    (((uint32_t)0x1)<<UNIT_CONFIG_MODE_TREND_150CYCLES_POS)
#define UNIT_CONFIG_MODE_TREND_150CYCLES_ENABLE  (((uint32_t)0x1)<<UNIT_CONFIG_MODE_TREND_150CYCLES_POS)
#define UNIT_CONFIG_MODE_TREND_150CYCLES_DISABLE (((uint32_t)0x0)<<UNIT_CONFIG_MODE_TREND_150CYCLES_POS)
// enable send of 150 cycles trend to ARM
#define UNIT_CONFIG_MODE_TREND_5MINUTES_POS      (14)
#define UNIT_CONFIG_MODE_TREND_5MINUTES_MASK     (((uint32_t)0x1)<<UNIT_CONFIG_MODE_TREND_5MINUTES_POS)
#define UNIT_CONFIG_MODE_TREND_5MINUTES_ENABLE   (((uint32_t)0x1)<<UNIT_CONFIG_MODE_TREND_5MINUTES_POS)
#define UNIT_CONFIG_MODE_TREND_5MINUTES_DISABLE  (((uint32_t)0x0)<<UNIT_CONFIG_MODE_TREND_5MINUTES_POS)
// enable send of event to ARM
#define UNIT_CONFIG_MODE_EVENT_POS               (15)
#define UNIT_CONFIG_MODE_EVENT_MASK              (((uint32_t)0x1)<<UNIT_CONFIG_MODE_EVENT_POS)
#define UNIT_CONFIG_MODE_EVENT_ENABLE            (((uint32_t)0x1)<<UNIT_CONFIG_MODE_EVENT_POS)
#define UNIT_CONFIG_MODE_EVENT_DISABLE           (((uint32_t)0x0)<<UNIT_CONFIG_MODE_EVENT_POS)
// Discard oldest months when SD-Card is Full
#define UNIT_CONFIG_MODE_SDCARD_DISCARD_POS      (16)
#define UNIT_CONFIG_MODE_SDCARD_DISCARD_MASK     (((uint32_t)0x1)<<UNIT_CONFIG_MODE_SDCARD_DISCARD_POS)
#define UNIT_CONFIG_MODE_SDCARD_DISCARD_ENABLE   (((uint32_t)0x1)<<UNIT_CONFIG_MODE_SDCARD_DISCARD_POS)
#define UNIT_CONFIG_MODE_SDCARD_DISCARD_DISABLE  (((uint32_t)0x0)<<UNIT_CONFIG_MODE_SDCARD_DISCARD_POS)

// event flags (32 bits unsigned)
#define UNIT_CONFIG_EVENT_DISABLE_MASK           ((uint32_t)(0u<<0))
#define UNIT_CONFIG_EVENT_RMS_OV_V1_MASK         ((uint32_t)(1u<<0))
#define UNIT_CONFIG_EVENT_RMS_OV_V2_MASK         ((uint32_t)(1u<<1))
#define UNIT_CONFIG_EVENT_RMS_OV_V3_MASK         ((uint32_t)(1u<<2))
#define UNIT_CONFIG_EVENT_RMS_UV_V1_MASK         ((uint32_t)(1u<<3))
#define UNIT_CONFIG_EVENT_RMS_UV_V2_MASK         ((uint32_t)(1u<<4))
#define UNIT_CONFIG_EVENT_RMS_UV_V3_MASK         ((uint32_t)(1u<<5))
#define UNIT_CONFIG_EVENT_FREQ_UF_MASK           ((uint32_t)(1u<<6))
#define UNIT_CONFIG_EVENT_FREQ_OF_MASK           ((uint32_t)(1u<<7))
#define UNIT_CONFIG_EVENT_RMS_OC_I1_MASK         ((uint32_t)(1u<<8))
#define UNIT_CONFIG_EVENT_RMS_OC_I2_MASK         ((uint32_t)(1u<<9))
#define UNIT_CONFIG_EVENT_RMS_OC_I3_MASK         ((uint32_t)(1u<<10))

// unit configuration
typedef struct {

	// General
	uint32_t mode_flags;
	uint32_t event_flags;

	// Sampling period set point
	int32_t Ts_ticks_Q5_SP;

	// Sampling period controller
	int32_t Controller_G0_Q10;
	int32_t Controller_G1_Q10;
	int32_t Controller_G2_Q10;
	int32_t DTs_ticks_Q5_MAX;
	int32_t DTs_ticks_Q5_MIN;
	
	// channel gain
	int32_t gain_volt;
	int32_t gain_curr_A;
	int32_t gain_curr_B;
		
	// sd-card memory
	int32_t sdcard_memory_max_MB;
	
	// Event detection and snapshot time
	int32_t event_RMS2_over_voltage_threshold;
	int32_t event_RMS2_under_voltage_threshold;
	int32_t event_RMS2_over_current_threshold;
	int32_t event_FREQ_over_freq_threshold;
	int32_t event_FREQ_under_freq_threshold;
	
} unit_config_t;

//*****************************************************************************
// unit calibration
//*****************************************************************************

typedef struct {

	// ADC Calibration
	int32_t adc_ch_gain_Q15[CH_ADC_GRID_TOTAL_NUM];
	int32_t adc_ch_offset[CH_ADC_GRID_TOTAL_NUM];

} unit_calibration_t;

//*****************************************************************************
// Default parameters
//*****************************************************************************
#define DEFAULT_MODE_FLAGS               UNIT_CONFIG_MODE_TS_FSPT |                  \
                                         UNIT_CONFIG_MODE_ADC_EMUL |                 \
                                         UNIT_CONFIG_MODE_CONTROLLER_PI |            \
                                         UNIT_CONFIG_MODE_TREND_150CYCLES_DISABLE |  \
                                         UNIT_CONFIG_MODE_TREND_5MINUTES_DISABLE |   \
                                         UNIT_CONFIG_MODE_EVENT_DISABLE | \
										 UNIT_CONFIG_MODE_SDCARD_DISCARD_ENABLE;
#define DEFAULT_EVENT_FLAGS              UNIT_CONFIG_EVENT_RMS_OV_V1_MASK | UNIT_CONFIG_EVENT_RMS_OV_V2_MASK | UNIT_CONFIG_EVENT_RMS_OV_V3_MASK | \
                                         UNIT_CONFIG_EVENT_RMS_UV_V1_MASK | UNIT_CONFIG_EVENT_RMS_UV_V2_MASK | UNIT_CONFIG_EVENT_RMS_UV_V3_MASK | \
                                         UNIT_CONFIG_EVENT_FREQ_UF_MASK   | UNIT_CONFIG_EVENT_FREQ_OF_MASK   |                                    \
                                         UNIT_CONFIG_EVENT_RMS_OC_I1_MASK | UNIT_CONFIG_EVENT_RMS_OC_I2_MASK | UNIT_CONFIG_EVENT_RMS_OC_I3_MASK;
#define DEFAULT_TS_TICKS_Q5_SP           TOFIX( SETTINGS_DSP_SYSCLK_HZ / ( 50.0 * SETTINGS_DSP_NPLL ) , 5 )
#define DEFAULT_CONTROLLER_G0_Q10        TOFIX( +1.730432838898867e-05 * SETTINGS_DSP_SYSCLK_HZ , 10 )
#define DEFAULT_CONTROLLER_G1_Q10        TOFIX( -1.726021465392558e-05 * SETTINGS_DSP_SYSCLK_HZ , 10 )
#define DEFAULT_CONTROLLER_G2_Q10        TOFIX( 0 , 10 )
#define DEFAULT_DTS_TICKS_Q5_MAX         TOFIX( +20e-6 * SETTINGS_DSP_SYSCLK_HZ , 5 )
#define DEFAULT_DTS_TICKS_Q5_MIN         TOFIX( -20e-6 * SETTINGS_DSP_SYSCLK_HZ , 5 )
#define DEFAULT_GAIN                     Q15  // gain = 1
#define DEFAULT_OFFSET                   -Q15
#define DEFAULT_GAIN_VOLT                (400)
#define DEFAULT_GAIN_CURR_A              (400)
#define DEFAULT_GAIN_CURR_B              (400)
#define DEFAULT_SDCARD_MEMORY_MB         (8*1024)
#define DEFAULT_EVENT_RMS_OV_THR         SQUARE( 1.1 * 220.0 * 32768.0 / DEFAULT_GAIN_VOLT); // 110% of 220Vrms
#define DEFAULT_EVENT_RMS_UV_THR         SQUARE( 0.9 * 220.0 * 32768.0 / DEFAULT_GAIN_VOLT); // 90% of 220Vrms
#define DEFAULT_EVENT_RMS_OC_THR         SQUARE( 150.0 * 32768.0 / DEFAULT_GAIN_CURR_A); // 150Arms
#define DEFAULT_EVENT_RMS_OF_THR         (52)
#define DEFAULT_EVENT_RMS_UF_THR         (48)

//#define DEFAULT_NDFT                     TOFIX( 2.0 , 0 )
//#define DEFAULT_EVENT_THRESHOLD          TOFIX( 30000.0 , 0 ) // todo: define!!!!
//#define DEFAULT_SNAPSHOT_MS              TOFIX( 10000.0 , 0 ) // snapshot each 1 second

#endif // __UNIT_COMMON_H__
