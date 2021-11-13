// $Header
/// @file			trend_common.h
/// @brief 			Settings for DSP and ARM.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 March 2017
/// @details
/// @copyright
///

#ifndef __TREND_COMMON_H__
#define __TREND_COMMON_H__

//*****************************************************************************
// Trends parameters
//*****************************************************************************

#define SETTINGS_TREND_MSG_PAYLOAD_BYTE_MAX 	(128)  // max payload length by msg
#define SETTINGS_INDEX_10CYCLE_ACCUM_MAX_DATA   (1280) // 10 cycles x 128 samples/cycles = 1280 samples
#define SETTINGS_TREND_150CYCLE_ACCUM_MAX_DATA  (15)   // 150 cycles / 10 cycles = 15

//#define SETTINGS_TREND_LENGTH_IN_BYTE	    	((uint16_t)(5*SETTINGS_TREND_MSG_PAYLOAD_BYTE_MAX + 116)) // length of trend
//#define SETTINGS_TREND_LENGTH_IN_BYTE	    	((uint16_t)sizeof(trend_t)) // length of trend
#define SETTINGS_TREND_LENGTH_IN_BYTE	    	((uint16_t)(9*SETTINGS_TREND_MSG_PAYLOAD_BYTE_MAX + 24)) // length of trend

//#define SETTINGS_TREND_LENGHT_IN_MSG        	((uint16_t)(1 + 5 + 1 + 1)) // start + 6 data (128 byte) + 21 data rest (<128) + stop
//#define SETTINGS_TREND_LENGHT_IN_MSG        	((uint16_t)(1 + (uint16_t)(sizeof(trend_t)/SETTINGS_TREND_MSG_PAYLOAD_BYTE_MAX) + 1 + 1)) // start + X data (128 byte) + data rest (<128) + stop
#define SETTINGS_TREND_LENGHT_IN_MSG        	((uint16_t)(1 + 9 + 1 + 1)) // start + 9 data (128 byte) + data rest (<128) + stop

//*****************************************************************************
// Trend flags
//*****************************************************************************

#define SETTINGS_TREND_FLAGS_INIT				        ((uint16_t)(1u<<0)) // system init
#define SETTINGS_TREND_FLAGS_UNIT_CONFIG_UPDATE			((uint16_t)(1u<<1))
#define SETTINGS_TREND_FLAGS_UNIT_CALIBRATION_UPDATE	((uint16_t)(1u<<2))
#define SETTINGS_TREND_FLAGS_RTC_UPDATE			        ((uint16_t)(1u<<3))
#define SETTINGS_TREND_FLAGS_EVENT_START		        ((uint16_t)(1u<<4))
#define SETTINGS_TREND_FLAGS_EVENT_END			        ((uint16_t)(1u<<5))
#define SETTINGS_TREND_FLAGS_LOW_VIN			        ((uint16_t)(1u<<6))

//*****************************************************************************
// Trend structure definition
//*****************************************************************************

// Real Time clock variable Definition
typedef struct {

	uint16_t m_second;	  // 14bits (0..999)
	uint16_t second;	  // 6bits  (0..59)
	uint16_t minute;	  // 6bits  (0..59)
	uint16_t hour;		  // 5bits  (0..23)

} trend_rtc_t;

typedef struct {

	int16_t real[3];
	int16_t imag[3];

} trend_complex16_t;

typedef struct {

	// Accumulator of signal^2
	int16_t rms[3];

	// Accumulator for harmonics
	trend_complex16_t harmonic[SETTINGS_DSP_NDFT_MAX+1]; // harmonics + DC value

	// (Total waveform distortion)^2
	int16_t twd[3];

} trend_result_ch_t;

typedef struct {

	int16_t ApparentPower[3];
	int16_t ActivePower[3];
	int16_t NoactivePower[3];
	int16_t PowerFactor[3];

} trend_result_ch_power_t;

typedef struct {

	trend_complex16_t pos;
	trend_complex16_t neg;
	trend_complex16_t hom;

} trend_result_seq_t;

typedef struct {

	// time
	trend_rtc_t time;

	// flags
	uint16_t flags;

	// maximum time utilization
	int16_t time_utilization;

	// sampling period
	int16_t Ts[3];

	// frequency
	int16_t fL[3];

	// channel
	trend_result_ch_t ch[CH_ADC_GRID_TOTAL_NUM];

	// sequences
	trend_result_seq_t v_seq;
	trend_result_seq_t iA_seq;
	trend_result_seq_t iB_seq;

	// unbalance
	int16_t v_desb_neg[3];
	int16_t v_desb_hom[3];
	int16_t iA_desb_neg[3];
	int16_t iA_desb_hom[3];
	int16_t iB_desb_neg[3];
	int16_t iB_desb_hom[3];

	// Power
	trend_result_ch_power_t ch_power[CH_POWER_TOTAL_NUM];

} trend_t;

#endif // __TREND_COMMON_H__
