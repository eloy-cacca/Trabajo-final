// $Header
/// @file			event_common.h
/// @brief 			Settings for DSP and ARM.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 March 2017
/// @details
/// @copyright
///

#ifndef __EVENT_COMMON_H__
#define __EVENT_COMMON_H__


//*****************************************************************************
// ARM and DSP - Event parameters
//*****************************************************************************

#define SETTINGS_EVENT_BUFFER_NUM           ((uint16_t)9)         // Buffer column. Data.
#define SETTINGS_EVENT_BUFFER_LENGHT        ((uint16_t)1024)      // Buffer row. Buffer length of 16 bits (4^5 = 1024).
//#define SETTINGS_EVENT_BUFFER_ROW_BY_MSG    ((uint16_t)4)         // Number of row buffer by event msg request.
//#define SETTINGS_EVENT_LENGHT_IN_MSG	    ((uint16_t)( (SETTINGS_EVENT_BUFFER_LENGHT/SETTINGS_EVENT_BUFFER_ROW_BY_MSG) + 2 ))  // Number of event msg request.
//#define SETTINGS_EVENT_LENGTH_IN_BYTE       ((uint16_t)( SETTINGS_EVENT_BUFFER_ROW_BY_MSG * SETTINGS_EVENT_BUFFER_NUM * 2 ))     // Number of byte by msg
//#define SETTINGS_EVENT_LENGTH_INFO_IN_BYTE	((uint16_t)( 10 ))


#define SETTINGS_EVENT_BUFFER_B1_START      ((uint16_t)( 0                                                                      ))		// 0
#define SETTINGS_EVENT_BUFFER_B1_STOP       ((uint16_t)( SETTINGS_EVENT_BUFFER_B1_START - 1 + (SETTINGS_EVENT_BUFFER_LENGHT>>2) ))		// 255
#define SETTINGS_EVENT_BUFFER_B2_START      ((uint16_t)( SETTINGS_EVENT_BUFFER_B1_STOP  + 1                                     ))		// 256
#define SETTINGS_EVENT_BUFFER_B2_STOP       ((uint16_t)( SETTINGS_EVENT_BUFFER_B2_START - 1 + (SETTINGS_EVENT_BUFFER_LENGHT>>2) ))		// 511
#define SETTINGS_EVENT_BUFFER_B3_START      ((uint16_t)( SETTINGS_EVENT_BUFFER_B2_STOP  + 1                                     ))		// 512
#define SETTINGS_EVENT_BUFFER_B3_STOP       ((uint16_t)( SETTINGS_EVENT_BUFFER_B3_START - 1 + (SETTINGS_EVENT_BUFFER_LENGHT>>2) ))		// 767
#define SETTINGS_EVENT_BUFFER_B4_START      ((uint16_t)( SETTINGS_EVENT_BUFFER_B3_STOP  + 1                                     ))		// 768
#define SETTINGS_EVENT_BUFFER_B4_STOP       ((uint16_t)( SETTINGS_EVENT_BUFFER_B4_START - 1 + (SETTINGS_EVENT_BUFFER_LENGHT>>2) ))		// 1023

//*****************************************************************************
// ARM and DSP - event definition
//*****************************************************************************

// event state machine
enum event_buffer_state_enum{
	EVENT_BUFFER_SAVE_B1 = 0,  // save data into B1
	EVENT_BUFFER_SAVE_B2B3,    // save data into B2 and B3
	EVENT_BUFFER_SAVE_B3,      // save data into B3
	EVENT_BUFFER_SAVE_B4,      // save data into B4
	EVENT_BUFFER_WAITTING_READ // waiting that ARM read data
};

// event info
typedef struct {

	uint16_t  type;
	trend_rtc_t time;

} event_info_t;

// event structure
typedef struct {

	enum event_buffer_state_enum  state;

	int16_t buffer[SETTINGS_EVENT_BUFFER_LENGHT][SETTINGS_EVENT_BUFFER_NUM];

	// implement index with signed counter since they can be negative in some part of code
	int16_t index;
	int16_t index_start;
	int16_t index_stop;
	int16_t det_B1_index;
	int16_t det_stop_index;
	int16_t buffer_divided;

} event_buffer_t;

// data stored in event buffers
enum event_data_enum{
	TS_TICKS = 0,
	V1_AMP,
	V2_AMP,
	V3_AMP,
	VE_AMP,
	I1_AMP,
	I2_AMP,
	I3_AMP,
	IN_AMP
};

// Event type
#define EVENT_TYPE_RMS_OV_V1_MASK         (((uint32_t)1u)<<0)
#define EVENT_TYPE_RMS_OV_V2_MASK         (((uint32_t)1u)<<1)
#define EVENT_TYPE_RMS_OV_V3_MASK         (((uint32_t)1u)<<2)
#define EVENT_TYPE_RMS_UV_V1_MASK         (((uint32_t)1u)<<3)
#define EVENT_TYPE_RMS_UV_V2_MASK         (((uint32_t)1u)<<4)
#define EVENT_TYPE_RMS_UV_V3_MASK         (((uint32_t)1u)<<5)
#define EVENT_TYPE_RMS_OC_I1_MASK         (((uint32_t)1u)<<6)
#define EVENT_TYPE_RMS_OC_I2_MASK         (((uint32_t)1u)<<7)
#define EVENT_TYPE_RMS_OC_I3_MASK         (((uint32_t)1u)<<8)
#define EVENT_TYPE_FREQ_UF_MASK           (((uint32_t)1u)<<9)
#define EVENT_TYPE_FREQ_OF_MASK           (((uint32_t)1u)<<10)
#define EVENT_TYPE_BUFFER_USED_MASK       (((uint32_t)1u)<<11) // this flag is used to identify the recording of data into buffer in the present detected event

//*****************************************************************************
// Event detection
//*****************************************************************************

// event detection finite state machine
enum event_det_state_enum{
	EVENT_DET_STARTING = 0,	// Ignore any events until system is started
	EVENT_DET_START,		// Check start of event
	EVENT_DET_END,			// Check end of event
	EVENT_DET_BACKGROUND_1, // Wait background to complete processing
	EVENT_DET_BACKGROUND_2  // Send data to ARM, wait response and clear variables
};

// event structure
typedef struct {

	uint16_t    state;
	uint16_t    type;
	uint32_t    duration_dsp_ticks;
	int16_t     v_max;
	int16_t     v_min;
	int16_t     i_max;
	trend_rtc_t time;

} event_det_t;

#endif // __EVENT_COMMON_H__
