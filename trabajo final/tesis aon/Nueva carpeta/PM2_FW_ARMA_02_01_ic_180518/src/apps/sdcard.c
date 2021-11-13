// $Header
/// @file			sdcard.c
/// @brief  		SD Card functions.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 December 2016
/// @details
/// @copyright
///

/// @addtogroup apps
/// @{

/**************************** INCLUDES ***************************************/

#include "sdcard.h"

#include "unit_config.h"
#include "system.h"
#include "../utils/iqmath.h"
#include "../utils/utils.h"
#include "../utils/calculus.h"
#include "../apis/rtc_api.h"
#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

//#define TRENDS_LABELS "ARM_time,DSP_time,flags,time_utilization,Ts_us_avg,Ts_us_max,Ts_us_min,fL_Hz_avg,fL_Hz_max,fL_Hz_min,V1_RMS_avg,V1_RMS_max,V1_RMS_min,V1H0_mod_avg,V1H0_mod_max,V1H0_mod_min,V1H0_angle_avg,V1H0_angle_max,V1H0_angle_min,V1H1_mod_avg,V1H1_mod_max,V1H1_mod_min,V1H1_angle_avg,V1H1_angle_max,V1H1_angle_min,V1H3_mod_avg,V1H3_mod_max,V1H3_mod_min,V1H3_angle_avg,V1H3_angle_max,V1H3_angle_min,V1H5_mod_avg,V1H5_mod_max,V1H5_mod_min,V1H5_angle_avg,V1H5_angle_max,V1H5_angle_min,V1H7_mod_avg,V1H7_mod_max,V1H7_mod_min,V1H7_angle_avg,V1H7_angle_max,V1H7_angle_min,V1_TWD_avg,V1_TWD_max,V1_TWD_min,V2_RMS_avg,V2_RMS_max,V2_RMS_min,V2H0_mod_avg,V2H0_mod_max,V2H0_mod_min,V2H0_angle_avg,V2H0_angle_max,V2H0_angle_min,V2H1_mod_avg,V2H1_mod_max,V2H1_mod_min,V2H1_angle_avg,V2H1_angle_max,V2H1_angle_min,V2H3_mod_avg,V2H3_mod_max,V2H3_mod_min,V2H3_angle_avg,V2H3_angle_max,V2H3_angle_min,V2H5_mod_avg,V2H5_mod_max,V2H5_mod_min,V2H5_angle_avg,V2H5_angle_max,V2H5_angle_min,V2H7_mod_avg,V2H7_mod_max,V2H7_mod_min,V2H7_angle_avg,V2H7_angle_max,V2H7_angle_min,V2_TWD_avg,V2_TWD_max,V2_TWD_min,V3_RMS_avg,V3_RMS_max,V3_RMS_min,V3H0_mod_avg,V3H0_mod_max,V3H0_mod_min,V3H0_angle_avg,V3H0_angle_max,V3H0_angle_min,V3H1_mod_avg,V3H1_mod_max,V3H1_mod_min,V3H1_angle_avg,V3H1_angle_max,V3H1_angle_min,V3H3_mod_avg,V3H3_mod_max,V3H3_mod_min,V3H3_angle_avg,V3H3_angle_max,V3H3_angle_min,V3H5_mod_avg,V3H5_mod_max,V3H5_mod_min,V3H5_angle_avg,V3H5_angle_max,V3H5_angle_min,V3H7_mod_avg,V3H7_mod_max,V3H7_mod_min,V3H7_angle_avg,V3H7_angle_max,V3H7_angle_min,V3_TWD_avg,V3_TWD_max,V3_TWD_min,VE_RMS_avg,VE_RMS_max,VE_RMS_min,VEH0_mod_avg,VEH0_mod_max,VEH0_mod_min,VEH0_angle_avg,VEH0_angle_max,VEH0_angle_min,VEH1_mod_avg,VEH1_mod_max,VEH1_mod_min,VEH1_angle_avg,VEH1_angle_max,VEH1_angle_min,VEH3_mod_avg,VEH3_mod_max,VEH3_mod_min,VEH3_angle_avg,VEH3_angle_max,VEH3_angle_min,VEH5_mod_avg,VEH5_mod_max,VEH5_mod_min,VEH5_angle_avg,VEH5_angle_max,VEH5_angle_min,VEH7_mod_avg,VEH7_mod_max,VEH7_mod_min,VEH7_angle_avg,VEH7_angle_max,VEH7_angle_min,VE_TWD_avg,VE_TWD_max,VE_TWD_min,I1_RMS_avg,I1_RMS_max,I1_RMS_min,I1H0_mod_avg,I1H0_mod_max,I1H0_mod_min,I1H0_angle_avg,I1H0_angle_max,I1H0_angle_min,I1H1_mod_avg,I1H1_mod_max,I1H1_mod_min,I1H1_angle_avg,I1H1_angle_max,I1H1_angle_min,I1H3_mod_avg,I1H3_mod_max,I1H3_mod_min,I1H3_angle_avg,I1H3_angle_max,I1H3_angle_min,I1H5_mod_avg,I1H5_mod_max,I1H5_mod_min,I1H5_angle_avg,I1H5_angle_max,I1H5_angle_min,I1H7_mod_avg,I1H7_mod_max,I1H7_mod_min,I1H7_angle_avg,I1H7_angle_max,I1H7_angle_min,I1_TWD_avg,I1_TWD_max,I1_TWD_min,I2_RMS_avg,I2_RMS_max,I2_RMS_min,I2H0_mod_avg,I2H0_mod_max,I2H0_mod_min,I2H0_angle_avg,I2H0_angle_max,I2H0_angle_min,I2H1_mod_avg,I2H1_mod_max,I2H1_mod_min,I2H1_angle_avg,I2H1_angle_max,I2H1_angle_min,I2H3_mod_avg,I2H3_mod_max,I2H3_mod_min,I2H3_angle_avg,I2H3_angle_max,I2H3_angle_min,I2H5_mod_avg,I2H5_mod_max,I2H5_mod_min,I2H5_angle_avg,I2H5_angle_max,I2H5_angle_min,I2H7_mod_avg,I2H7_mod_max,I2H7_mod_min,I2H7_angle_avg,I2H7_angle_max,I2H7_angle_min,I2_TWD_avg,I2_TWD_max,I2_TWD_min,IN_RMS_avg,IN_RMS_max,IN_RMS_min,INH0_mod_avg,INH0_mod_max,INH0_mod_min,INH0_angle_avg,INH0_angle_max,INH0_angle_min,INH1_mod_avg,INH1_mod_max,INH1_mod_min,INH1_angle_avg,INH1_angle_max,INH1_angle_min,INH3_mod_avg,INH3_mod_max,INH3_mod_min,INH3_angle_avg,INH3_angle_max,INH3_angle_min,INH5_mod_avg,INH5_mod_max,INH5_mod_min,INH5_angle_avg,INH5_angle_max,INH5_angle_min,INH7_mod_avg,INH7_mod_max,INH7_mod_min,INH7_angle_avg,INH7_angle_max,INH7_angle_min,IN_TWD_avg,IN_TWD_max,IN_TWD_min,VPOS_mod_avg,VPOS_mod_max,VPOS_mod_min,VPOS_angle_avg,VPOS_angle_max,VPOS_angle_min,VNEG_mod_avg,VNEG_mod_max,VNEG_mod_min,VNEG_angle_avg,VNEG_angle_max,VNEG_angle_min,VHOM_mod_avg,VHOM_mod_max,VHOM_mod_min,VHOM_angle_avg,VHOM_angle_max,VHOM_angle_min,IPOS_mod_avg,IPOS_mod_max,IPOS_mod_min,IPOS_angle_avg,IPOS_angle_max,IPOS_angle_min,INEG_mod_avg,INEG_mod_max,INEG_mod_min,INEG_angle_avg,INEG_angle_max,INEG_angle_min,IHOM_mod_avg,IHOM_mod_max,IHOM_mod_min,IHOM_angle_avg,IHOM_angle_max,IHOM_angle_min,DES_NEG_mod_avg,DES_NEG_mod_max,DES_NEG_mod_min,DES_NEG_angle_avg,DES_NEG_angle_max,DES_NEG_angle_min,DES_HOM_mod_avg,DES_HOM_mod_max,DES_HOM_mod_min,DES_HOM_angle_avg,DES_HOM_angle_max,DES_HOM_angle_min,P1_APPARENT_avg,P1_APPARENT_max,P1_APPARENT_min,P1_ACTIVE_avg,P1_ACTIVE_max,P1_ACTIVE_min,P1_NOACTIVE_avg,P1_NOACTIVE_max,P1_NOACTIVE_min,P1_PF_avg,P1_PF_max,P1_PF_mod_min,P2_APPARENT_avg,P2_APPARENT_max,P2_APPARENT_min,P2_ACTIVE_avg,P2_ACTIVE_max,P2_ACTIVE_min,P2_NOACTIVE_avg,P2_NOACTIVE_max,P2_NOACTIVE_min,P2_PF_avg,P2_PF_max,P2_PF_mod_min,P3_APPARENT_avg,P3_APPARENT_max,P3_APPARENT_min,P3_ACTIVE_avg,P3_ACTIVE_max,P3_ACTIVE_min,P3_NOACTIVE_avg,P3_NOACTIVE_max,P3_NOACTIVE_min,P3_PF_avg,P3_PF_max,P3_PF_mod_min\n"
#define TRENDS_LABELS "\
ARM_time,DSP_time,flags,time_utilization,\
Ts_us_avg,Ts_us_max,Ts_us_min,\
fL_Hz_avg,fL_Hz_max,fL_Hz_min,\
V1_RMS_avg,V1_RMS_max,V1_RMS_min,\
V1H0_mod_avg,V1H0_mod_max,V1H0_mod_min,V1H0_angle_avg,V1H0_angle_max,V1H0_angle_min,\
V1H1_mod_avg,V1H1_mod_max,V1H1_mod_min,V1H1_angle_avg,V1H1_angle_max,V1H1_angle_min,\
V1H3_mod_avg,V1H3_mod_max,V1H3_mod_min,V1H3_angle_avg,V1H3_angle_max,V1H3_angle_min,\
V1H5_mod_avg,V1H5_mod_max,V1H5_mod_min,V1H5_angle_avg,V1H5_angle_max,V1H5_angle_min,\
V1H7_mod_avg,V1H7_mod_max,V1H7_mod_min,V1H7_angle_avg,V1H7_angle_max,V1H7_angle_min,\
V1_TWD_avg,V1_TWD_max,V1_TWD_min,\
V2_RMS_avg,V2_RMS_max,V2_RMS_min,\
V2H0_mod_avg,V2H0_mod_max,V2H0_mod_min,V2H0_angle_avg,V2H0_angle_max,V2H0_angle_min,\
V2H1_mod_avg,V2H1_mod_max,V2H1_mod_min,V2H1_angle_avg,V2H1_angle_max,V2H1_angle_min,\
V2H3_mod_avg,V2H3_mod_max,V2H3_mod_min,V2H3_angle_avg,V2H3_angle_max,V2H3_angle_min,\
V2H5_mod_avg,V2H5_mod_max,V2H5_mod_min,V2H5_angle_avg,V2H5_angle_max,V2H5_angle_min,\
V2H7_mod_avg,V2H7_mod_max,V2H7_mod_min,V2H7_angle_avg,V2H7_angle_max,V2H7_angle_min,\
V2_TWD_avg,V2_TWD_max,V2_TWD_min,\
V3_RMS_avg,V3_RMS_max,V3_RMS_min,\
V3H0_mod_avg,V3H0_mod_max,V3H0_mod_min,V3H0_angle_avg,V3H0_angle_max,V3H0_angle_min,\
V3H1_mod_avg,V3H1_mod_max,V3H1_mod_min,V3H1_angle_avg,V3H1_angle_max,V3H1_angle_min,\
V3H3_mod_avg,V3H3_mod_max,V3H3_mod_min,V3H3_angle_avg,V3H3_angle_max,V3H3_angle_min,\
V3H5_mod_avg,V3H5_mod_max,V3H5_mod_min,V3H5_angle_avg,V3H5_angle_max,V3H5_angle_min,\
V3H7_mod_avg,V3H7_mod_max,V3H7_mod_min,V3H7_angle_avg,V3H7_angle_max,V3H7_angle_min,\
V3_TWD_avg,V3_TWD_max,V3_TWD_min,\
VE_RMS_avg,VE_RMS_max,VE_RMS_min,\
VEH0_mod_avg,VEH0_mod_max,VEH0_mod_min,VEH0_angle_avg,VEH0_angle_max,VEH0_angle_min,\
VEH1_mod_avg,VEH1_mod_max,VEH1_mod_min,VEH1_angle_avg,VEH1_angle_max,VEH1_angle_min,\
VEH3_mod_avg,VEH3_mod_max,VEH3_mod_min,VEH3_angle_avg,VEH3_angle_max,VEH3_angle_min,\
VEH5_mod_avg,VEH5_mod_max,VEH5_mod_min,VEH5_angle_avg,VEH5_angle_max,VEH5_angle_min,\
VEH7_mod_avg,VEH7_mod_max,VEH7_mod_min,VEH7_angle_avg,VEH7_angle_max,VEH7_angle_min,\
VE_TWD_avg,VE_TWD_max,VE_TWD_min,\
I1_RMS_avg,I1_RMS_max,I1_RMS_min,\
I1H0_mod_avg,I1H0_mod_max,I1H0_mod_min,I1H0_angle_avg,I1H0_angle_max,I1H0_angle_min,\
I1H1_mod_avg,I1H1_mod_max,I1H1_mod_min,I1H1_angle_avg,I1H1_angle_max,I1H1_angle_min,\
I1H3_mod_avg,I1H3_mod_max,I1H3_mod_min,I1H3_angle_avg,I1H3_angle_max,I1H3_angle_min,\
I1H5_mod_avg,I1H5_mod_max,I1H5_mod_min,I1H5_angle_avg,I1H5_angle_max,I1H5_angle_min,\
I1H7_mod_avg,I1H7_mod_max,I1H7_mod_min,I1H7_angle_avg,I1H7_angle_max,I1H7_angle_min,\
I1_TWD_avg,I1_TWD_max,I1_TWD_min,\
I2_RMS_avg,I2_RMS_max,I2_RMS_min,\
I2H0_mod_avg,I2H0_mod_max,I2H0_mod_min,I2H0_angle_avg,I2H0_angle_max,I2H0_angle_min,\
I2H1_mod_avg,I2H1_mod_max,I2H1_mod_min,I2H1_angle_avg,I2H1_angle_max,I2H1_angle_min,\
I2H3_mod_avg,I2H3_mod_max,I2H3_mod_min,I2H3_angle_avg,I2H3_angle_max,I2H3_angle_min,\
I2H5_mod_avg,I2H5_mod_max,I2H5_mod_min,I2H5_angle_avg,I2H5_angle_max,I2H5_angle_min,\
I2H7_mod_avg,I2H7_mod_max,I2H7_mod_min,I2H7_angle_avg,I2H7_angle_max,I2H7_angle_min,\
I2_TWD_avg,I2_TWD_max,I2_TWD_min,\
I3_RMS_avg,I3_RMS_max,I3_RMS_min,\
I3H0_mod_avg,I3H0_mod_max,I3H0_mod_min,I3H0_angle_avg,I3H0_angle_max,I3H0_angle_min,\
I3H1_mod_avg,I3H1_mod_max,I3H1_mod_min,I3H1_angle_avg,I3H1_angle_max,I3H1_angle_min,\
I3H3_mod_avg,I3H3_mod_max,I3H3_mod_min,I3H3_angle_avg,I3H3_angle_max,I3H3_angle_min,\
I3H5_mod_avg,I3H5_mod_max,I3H5_mod_min,I3H5_angle_avg,I3H5_angle_max,I3H5_angle_min,\
I3H7_mod_avg,I3H7_mod_max,I3H7_mod_min,I3H7_angle_avg,I3H7_angle_max,I3H7_angle_min,\
I3_TWD_avg,I3_TWD_max,I3_TWD_min,\
IN_RMS_avg,IN_RMS_max,IN_RMS_min,\
INH0_mod_avg,INH0_mod_max,INH0_mod_min,INH0_angle_avg,INH0_angle_max,INH0_angle_min,\
INH1_mod_avg,INH1_mod_max,INH1_mod_min,INH1_angle_avg,INH1_angle_max,INH1_angle_min,\
INH3_mod_avg,INH3_mod_max,INH3_mod_min,INH3_angle_avg,INH3_angle_max,INH3_angle_min,\
INH5_mod_avg,INH5_mod_max,INH5_mod_min,INH5_angle_avg,INH5_angle_max,INH5_angle_min,\
INH7_mod_avg,INH7_mod_max,INH7_mod_min,INH7_angle_avg,INH7_angle_max,INH7_angle_min,\
IN_TWD_avg,IN_TWD_max,IN_TWD_min,\
VPOS_mod_avg,VPOS_mod_max,VPOS_mod_min,VPOS_angle_avg,VPOS_angle_max,VPOS_angle_min,\
VNEG_mod_avg,VNEG_mod_max,VNEG_mod_min,VNEG_angle_avg,VNEG_angle_max,VNEG_angle_min,\
VHOM_mod_avg,VHOM_mod_max,VHOM_mod_min,VHOM_angle_avg,VHOM_angle_max,VHOM_angle_min,\
IPOS_mod_avg,IPOS_mod_max,IPOS_mod_min,IPOS_angle_avg,IPOS_angle_max,IPOS_angle_min,\
INEG_mod_avg,INEG_mod_max,INEG_mod_min,INEG_angle_avg,INEG_angle_max,INEG_angle_min,\
IHOM_mod_avg,IHOM_mod_max,IHOM_mod_min,IHOM_angle_avg,IHOM_angle_max,IHOM_angle_min,\
DES_NEG_avg,DES_NEG_max,DES_NEG_min,\
DES_HOM_avg,DES_HOM_max,DES_HOM_min,\
P1_APPARENT_avg,P1_APPARENT_max,P1_APPARENT_min,P1_ACTIVE_avg,P1_ACTIVE_max,P1_ACTIVE_min,\
P1_NOACTIVE_avg,P1_NOACTIVE_max,P1_NOACTIVE_min,P1_PF_avg,P1_PF_max,P1_PF_mod_min,\
P2_APPARENT_avg,P2_APPARENT_max,P2_APPARENT_min,P2_ACTIVE_avg,P2_ACTIVE_max,P2_ACTIVE_min,\
P2_NOACTIVE_avg,P2_NOACTIVE_max,P2_NOACTIVE_min,P2_PF_avg,P2_PF_max,P2_PF_mod_min,\
P3_APPARENT_avg,P3_APPARENT_max,P3_APPARENT_min,P3_ACTIVE_avg,P3_ACTIVE_max,P3_ACTIVE_min,\
P3_NOACTIVE_avg,P3_NOACTIVE_max,P3_NOACTIVE_min,P3_PF_avg,P3_PF_max,P3_PF_mod_min\
\n"

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

void compute_mod_and_phase(trend_complex16_t * complex , int16_t * complex_mod , int16_t * complex_phase );

FRESULT sdcard_mount_logic_drive(void);

bool sdcard_check_and_read_unit_config(void);
bool sdcard_check_and_read_unit_calibration(void);

bool save_data_x3( int16_t * data , float gain );
bool save_data_x1( int16_t * data , float gain );
bool print_device_identification( void );
bool sdcard_save_trends( trend_t * trend , uint8_t type);
bool sdcard_save_event_det_info( event_det_t * event_det );
bool sdcard_save_event_buffer( uint16_t * info , int16_t * buffer);

void sdcard_find_older_directory( char * dir_name );
uint8_t sdcard_find_older_month(char * dir_name);
void sdcard_check_free_space_and_update_directory_to_remove(void);
void sdcard_remove_directory(void);

/**************************** LOCAL VARIABLES ********************************/

// FAT file system
FATFS fs; //File system object
Ctrl_status status;
FRESULT res;
FIL file_object;
DIR sdcard_directory;
FILINFO sdcard_file_info;

// strings (constant)
char const file_name_cal[18] = "0:calibration.ini"; // calibration file name
char const file_name_conf[14] = "0:config.ini";     // config file name
// strings (general purpose)
char string[STRING_MAX];
// strings (used for temporal save of event buffer)
char string_event_file_name[STRING_MAX];
// strings (directory to be removed)
char dir_remove_name[STRING_MAX];
char dir_remove_name_length = 0;


/**************************** GLOBAL VARIABLES *******************************/

// SD-Card state and pending task flags
volatile sdcard_flags_status_t sdcard_flags_status;
volatile sdcard_flags_task_t   sdcard_flags_task;

// temporal register for data
volatile trend_t      sdcard_trend_150cycles;
volatile trend_t      sdcard_trend_5minutes;
volatile event_det_t  sdcard_event_det;
volatile uint16_t     sdcard_event_buffer_info[3];
volatile int16_t      sdcard_event_buffer[SETTINGS_EVENT_BUFFER_NUM];

/**************************** LOCAL FUNCTIONS ********************************/

/// @brief Mount the logic drive
FRESULT sdcard_mount_logic_drive(void) {

	memset(&fs, 0, sizeof(FATFS));
	return f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
}

/*****************************************************************************/

void compute_mod_and_phase(trend_complex16_t * complex , int16_t * complex_mod , int16_t * complex_phase ) {

	for ( uint8_t i=0 ; i<3 ; i++ ) {

		complex_mod[i] = MATH_SQRT( SQUARE(complex->real[i]) + SQUARE(complex->imag[i]) );
		complex_phase[i] = ((360.0/6.283185307179586)*64) * MATH_ATAN2( complex->imag[i] , complex->real[i] );

	}
	
}

/// @brief Check SD-Card, read config.txt and update unit config
bool sdcard_check_and_read_unit_config(void) {

	// check if config.txt exist
	if (FR_OK == f_open( &file_object , (char const *)file_name_conf , FA_READ )) {

		// Read data (32 bits)
		for ( uint8_t i=0 ; i<(sizeof(unit_config)>>2) ; i++ ) {
			
			uint8_t buffer[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
		
			// read data from SD-Card and stop when a '\n' (new line) is detected
			f_gets( (TCHAR *)buffer , (int)12 , &file_object );
			
			// Find '\n' (new line) in the string and remplace it with a '\0' (NULL caracter)
			(*UTILS_Strchr( (char *)buffer , '\n' ) ) = '\0';
		
			// converter int32_t for a string and save value in the unit configuration
			UTILS_Atoi( (char *)buffer , &((int32_t *)&unit_config)[i] );
			
		}

	}
	else {
		
		// save unit configuration in the SD Card.
		if (1 != sdcard_save_unit_config())
			return 0;
		
	}

	return 1;

}

/// @brief Check SD-Card, read calibration.txt and update unit calibration
bool sdcard_check_and_read_unit_calibration(void) {

	// check if config.txt exist
	if (FR_OK == f_open( &file_object , (char const *)file_name_cal , FA_READ )) {

		// Read data (32 bits)
		for ( uint8_t i=0 ; i<(sizeof(unit_calibration)>>2) ; i++ ) {
			
			uint8_t buffer[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
			
			// read data from SD-Card and stop when a '\n' (new line) is detected
			f_gets( (TCHAR *)buffer , (int)12 , &file_object );
			
			// Find '\n' (new line) in the string and remplace it with a '\0' (NULL caracter)
			(*UTILS_Strchr( (char *)buffer , '\n' ) ) = '\0';
			
			// converter int32_t for a string and save value in the unit configuration
			UTILS_Atoi( (char *)buffer , &((int32_t *)&unit_calibration)[i] );
			
		}

	}
	else {
		
		// save unit configuration in the SD Card.
		if (1 != sdcard_save_unit_calibration())
			return 0;
		
	}

	return 1;

}

/*****************************************************************************/

/// @brief Save 1 data
bool save_data_x1( int16_t * data , float gain ) {

	float temp;

	temp = data[0]*gain;
	if (EOF == f_printf( &file_object, "%d.%03d", (int16_t)temp , ABS((int16_t)((temp-(int32_t)temp)*1000.0)) )) return 0;

	return 1;
	
}

/// @brief Save max, min and average
bool save_data_x3( int16_t * data , float gain ) {

	float temp;

	temp = data[0]*gain;
	if (EOF == f_printf( &file_object, ",%d.%03d", (int16_t)temp , ABS((int16_t)((temp-(int32_t)temp)*1000.0)) )) return 0;
	temp = data[1]*gain;
	if (EOF == f_printf( &file_object, ",%d.%03d", (int16_t)temp , ABS((int16_t)((temp-(int32_t)temp)*1000.0)) )) return 0;
	temp = data[2]*gain;
	if (EOF == f_printf( &file_object, ",%d.%03d", (int16_t)temp , ABS((int16_t)((temp-(int32_t)temp)*1000.0)) )) return 0;
	
	return 1;

}

/// @brief print_device_label
bool print_device_identification( void ) {

	if (EOF ==  f_printf( &file_object, "%s (Project ID=0x%02x)\n", SETTINGS_PROJECT_NAME , SETTINGS_PROJECT_ID )) return 0;
	if (EOF ==  f_printf( &file_object, "ARM FW version = %d.%d\n", SETTINGS_ARM_FW_VERSION , SETTINGS_ARM_FW_VERSION_ENG_BUILD )) return 0;
	if (EOF ==  f_printf( &file_object, "DSP FW version = %d.%d\n", DSP_fw[0] , DSP_fw[1] )) return 0;
	if (EOF ==  f_puts( "Location name = none\n" , &file_object )) return 0;
	if (EOF ==  f_puts( "Device name = none\n" , &file_object )) return 0;
	if (EOF ==  f_puts( "Note_1 = none\n" , &file_object )) return 0;
	if (EOF ==  f_puts( "Note_2 = none\n" , &file_object )) return 0;
	if (EOF ==  f_puts( "Power configuration = Wye/Star\n" , &file_object )) return 0;
	if (EOF ==  f_puts( "Nominal Line-to-Line Voltage = 380V\n" , &file_object )) return 0;
	if (EOF ==  f_puts( "Nominal Line-to-Neutral Voltage = 220V\n\n" , &file_object )) return 0;
	
	return 1;

}

/// @brief Save trends into SD-Card.
//EXAMPLE: 12:50:43,13:01:13, 156.246, 156.246, 156.246, 226.391, 226.391, 226.391,   0.012,   0.012,   0.012, 180.000, 180.000, 180.000, 310.998, 310.998, 310.998, -90.000,   0.000, -90.000,  69.982,  69.982,  69.982,  50.625,  50.625,  50.625,   0.012,   0.012,   0.012, 180.000, 180.000, 180.000,  29.992,  29.992,  29.992,-140.625,   0.000,-140.625,  24.462,  24.462,  24.462,  95.117,  95.117,  95.117,   0.012,   0.012,   0.012, 180.000, 180.000, 180.000,  99.987,  99.987,  99.987, 149.062, 149.062, 149.062,   0.012,   0.012,   0.012, -90.000,   0.000, -90.000,   0.012,   0.012,   0.012, -90.000,   0.000, -90.000,  90.002,  90.002,  90.002,-140.609,   0.000,-140.609,  89.990,  89.990,  89.990,  45.251,  45.251,  45.251,   0.012,   0.012,   0.012, 180.000, 180.000, 180.000,  49.987,  49.987,  49.987,  30.921,  30.921,  30.921,  39.978,  39.978,  39.978,  50.609,  50.609,  50.609,   0.012,   0.012,   0.012, 180.000, 180.000, 180.000,   0.012,   0.012,   0.012,-135.000,   0.000,-135.000,  80.004,  80.004,  80.004,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.024,   0.024,   0.024,  84.844,  84.844,  84.844,   0.006,   0.006,   0.006, 180.000, 180.000, 180.000, 120.001, 120.001, 120.001, -90.000,   0.000, -90.000,   0.006,   0.006,   0.006, -90.000,   0.000, -90.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,  84.844,  84.844,  84.844,   0.006,   0.006,   0.006, 180.000, 180.000, 180.000, 119.995, 119.995, 119.995, 149.062, 149.062, 149.062,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.006,   0.006,   0.006, -90.000,   0.000, -90.000,   0.006,   0.006,   0.006,-135.000,   0.000,-135.000,   0.512,   0.512,   0.512,  84.844,  84.844,  84.844,   0.006,   0.006,   0.006, 180.000, 180.000, 180.000, 119.989, 119.989, 119.989,  30.921,  30.921,  30.921,   0.006,   0.006,   0.006, 180.000, 180.000, 180.000,   0.006,   0.006,   0.006,-135.000,   0.000,-135.000,   0.006,   0.006,   0.006, -90.000,   0.000, -90.000,   1.123,   1.123,   1.123,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.024,   0.024,   0.024, 153.649, 153.649, 153.649, -90.093,   0.000, -90.093,  80.688,  80.688,  80.688, -79.593,   0.000, -79.593,  79.260,  79.260,  79.260,-100.375,   0.000,-100.375,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,  52.490,  52.490,  52.490,  51.562,  51.562,  51.562,1000.000,1000.000,1000.000,1000.000,1000.000,1000.000,1000.000,1000.000,1000.000,1000.000,1000.000,1000.000
bool sdcard_save_trends( trend_t * trend , uint8_t type) {

	uint32_t year, month, day, hour, minute, second;
	float gain_voltage,gain_current,gain_power_Q14;
	int16_t complex_mod[6], complex_phase[6];
		
	// update gains
	gain_voltage = unit_config.gain_volt/SETTINGS_DSP_ADC_MAX_TICK;
	gain_current = unit_config.gain_curr_A/SETTINGS_DSP_ADC_MAX_TICK;
	gain_power_Q14 = gain_voltage*gain_current*Q14;
	
	// get date from RTC
	rtc_get_date(RTC, &year, &month, &day, 0);
	rtc_get_time(RTC, &hour, &minute, &second);
	
	// Define directory name (example "2015/Month_12/Day_01" for 1 of December of 2015) and create it.
	memset( (void *)&string , 0 , sizeof(string) );
	sprintf( (char *)&string ,"%4d" , (int16_t)year);
	f_mkdir(string);	// Create folder if it do not exist
	sprintf( (char *)&string+4 ,"/Month_%02d" , (int16_t)month);
	f_mkdir(string);	// Create folder if it do not exist
	sprintf( (char *)&string+13 ,"/Day_%02d" , (int16_t)day);
	f_mkdir(string);	// Create folder if it do not exist
	
	// define file name (example "PM_2015_12_05_DailyTrend_3s.txt" for 5 of December of 2015). File name is added to folder name so pointer is moved 20 chars.
	if (type == 0)
		sprintf( (char *)&string+20 ,"/LICPM2_%4d_%02d_%02d_DailyTrend_3s.txt" , (int16_t)year , (int16_t)month , (int16_t)day);
	else
		sprintf( (char *)&string+20 ,"/LICPM2_%4d_%02d_%02d_DailyTrend_5m.txt" , (int16_t)year , (int16_t)month , (int16_t)day);
		
	// get date from RTC. TODO: Por alguna razon que no entiendo sprintf sobrescribe la variable day. Repito la lectura del RTC hasta entender este problema.
	rtc_get_date(RTC, &year, &month, &day, 0);

	// open file or create it if it do not exist.
	if (FR_OK != f_open( &file_object , (char const *)string , FA_OPEN_ALWAYS | FA_WRITE ))	return 0;

	// if file was created, read/write pointer at begging, print Power Meter information
	if (f_tell ( &file_object ) == 0) {
		// device information
		if (EOF == print_device_identification()) return 0;
		// specific information
		if (EOF == f_printf( &file_object, "Daily Trend\n" )) return 0;
		if (EOF == f_puts( "Type = " , &file_object )) return 0;
		if (type == 0) 
			{ if (EOF == f_puts( "3 seconds" , &file_object )) return 0; }
		else
			{ if (EOF == f_puts( "5 minutes" , &file_object )) return 0; }
		if (EOF == f_printf( &file_object, "\nDate = %4d/%02d/%02d\n\n", (int16_t)year , (int16_t)month , (int16_t)day )) return 0;
		if (EOF == f_puts( TRENDS_LABELS , &file_object )) return 0;
	}
	
	// Move to end of the file to append data (needed when a existing file is open)
	if (FR_OK != f_lseek( &file_object , f_size(&file_object)) ) return 0;
	
	// Print ARM RTC for debugging porpuose.
	if (EOF == f_printf( &file_object, "%02d:%02d:%02d,", (int16_t)hour , (int16_t)minute , (int16_t)second )) return 0;
	
	// Print trends into sd-card
	{
		// Print time
		if (EOF == f_printf( &file_object, "%02d:%02d:%02d", trend->time.hour , trend->time.minute , trend->time.second )) return 0;

		// Print flags
		if (EOF == f_printf( &file_object, ",%016b", trend->flags )) return 0;

		// Print time utilization
		{
			float temp = trend->time_utilization * (1.0 / SETTINGS_DSP_SYSCLK_MHZ);
			if (EOF == f_printf( &file_object, ",%d.%03d", (int16_t)temp , ABS((int16_t)((temp-(int32_t)temp)*1000.0)) )) return 0;
		}
		
		// Print sampling period
		if (EOF == save_data_x3( &trend->Ts[0] , (1.0/SETTINGS_DSP_SYSCLK_MHZ) )) return 0;

		// Print frequency
		if (EOF == save_data_x3( &trend->fL[0] , (1.0/Q08) )) return 0;

		// Print RMS and harmonics accumulator
		{
			uint16_t CH_count;
			uint16_t H_count;

			for ( CH_count=0 ; CH_count<(SETTINGS_DSP_ADC_CH_NUMBER>>1) ; CH_count++ ) {

				// RMS
				if (EOF == save_data_x3( &trend->ch[CH_count].rms[0] , gain_voltage )) return 0;

				// Harmonics
				for ( H_count=0 ; H_count<(SETTINGS_DSP_NDFT_MAX+1) ; H_count++ ) {

					compute_mod_and_phase(&trend->ch[CH_count].harmonic[H_count] , &complex_mod[0] , &complex_phase[0] );
					if (EOF == save_data_x3( &complex_mod[0] , gain_voltage)) return 0;
					if (EOF == save_data_x3( &complex_phase[0] , (1.0/64.0) )) return 0;

				}

				// Total Waveform Distortion (TWD)
				if (EOF == save_data_x3( &trend->ch[CH_count].twd[0] , (100.0/4096.0) )) return 0;
			}

			for ( CH_count=(SETTINGS_DSP_ADC_CH_NUMBER>>1) ; CH_count<SETTINGS_DSP_ADC_CH_NUMBER ; CH_count++ ) {

				// RMS
				if (EOF == save_data_x3( &trend->ch[CH_count].rms[0] , gain_current )) return 0;

				// Harmonics
				for ( H_count=0 ; H_count<(SETTINGS_DSP_NDFT_MAX+1) ; H_count++ ) {
				
					compute_mod_and_phase(&trend->ch[CH_count].harmonic[H_count] , &complex_mod[0] , &complex_phase[0] );
					if (EOF == save_data_x3( &complex_mod[0] , gain_current)) return 0;
					if (EOF == save_data_x3( &complex_phase[0] , (1.0/64.0) )) return 0;

				}

				// Total Waveform Distortion (TWD)
				if (EOF == save_data_x3( &trend->ch[CH_count].twd[0] , (100.0/4096.0) )) return 0;
			}


		}

		// Print voltage 
		// positive sequence
		compute_mod_and_phase(&trend->v_seq.pos , &complex_mod[0] , &complex_phase[0] );
		if (EOF == save_data_x3( &complex_mod[0] , gain_voltage)) return 0;
		if (EOF == save_data_x3( &complex_phase[0] , (1.0/64.0) )) return 0;
		// negative sequence
		compute_mod_and_phase(&trend->v_seq.neg , &complex_mod[0] , &complex_phase[0] );
		if (EOF == save_data_x3( &complex_mod[0] , gain_voltage)) return 0;
		if (EOF == save_data_x3( &complex_phase[0] , (1.0/64.0) )) return 0;
		// homopolar sequence
		compute_mod_and_phase(&trend->v_seq.hom , &complex_mod[0] , &complex_phase[0] );
		if (EOF == save_data_x3( &complex_mod[0] , gain_voltage)) return 0;
		if (EOF == save_data_x3( &complex_phase[0] , (1.0/64.0) )) return 0;

		// Print current A
		// positive sequence
		compute_mod_and_phase(&trend->iA_seq.pos , &complex_mod[0] , &complex_phase[0] );
		if (EOF == save_data_x3( &complex_mod[0] , gain_current)) return 0;
		if (EOF == save_data_x3( &complex_phase[0] , (1.0/64.0) )) return 0;		
		// negative sequence
		compute_mod_and_phase(&trend->iA_seq.neg , &complex_mod[0] , &complex_phase[0] );
		if (EOF == save_data_x3( &complex_mod[0] , gain_current)) return 0;
		if (EOF == save_data_x3( &complex_phase[0] , (1.0/64.0) )) return 0;
		// homopolar sequence
		compute_mod_and_phase(&trend->iA_seq.hom , &complex_mod[0] , &complex_phase[0] );
		if (EOF == save_data_x3( &complex_mod[0] , gain_current)) return 0;
		if (EOF == save_data_x3( &complex_phase[0] , (1.0/64.0) )) return 0;

		// Print desbalance
		if (EOF == save_data_x3( &trend->v_desb_neg[0] , (100.0/4096.0) )) return 0;
		if (EOF == save_data_x3( &trend->v_desb_hom[0] , (100.0/4096.0) )) return 0;

		// Print power
		{
			uint16_t CH_count;

			for ( CH_count=0 ; CH_count<3 ; CH_count++ ) {

				if (EOF == save_data_x3( &trend->ch_power[CH_count].ApparentPower[0] , gain_power_Q14 )) return 0;
				if (EOF == save_data_x3( &trend->ch_power[CH_count].ActivePower[0]   , gain_power_Q14 )) return 0;
				if (EOF == save_data_x3( &trend->ch_power[CH_count].NoactivePower[0] , gain_power_Q14 )) return 0;
				if (EOF == save_data_x3( &trend->ch_power[CH_count].PowerFactor[0]   , (100.0/4096.0) )) return 0;

			}

		}
	
	}

	// Print new line
	if (EOF == f_puts( "\n" , &file_object )) return 0;
			
	// Close file
	if (FR_OK != f_close( &file_object )) return 0;

	return 1;
	
}

bool sdcard_save_event_det_info( event_det_t * event_det ) {

	uint32_t year, month, day;
	float gain_voltage,gain_current;
	
	// update gains
	gain_voltage = unit_config.gain_volt/SETTINGS_DSP_ADC_MAX_TICK;
	gain_current = unit_config.gain_curr_A/SETTINGS_DSP_ADC_MAX_TICK;
	
	// get date from RTC
	rtc_get_date(RTC, &year, &month, &day, 0);
	
	// Define directory name (example "2015/Month_12/Day_01" for 1 of December of 2015) and create it.
	memset( (void *)&string , 0 , sizeof(string) );
	sprintf( (char *)&string ,"%4d" , (int16_t)year);
	f_mkdir(string);	// Create folder if it do not exist
	sprintf( (char *)&string+4 ,"/Month_%02d" , (int16_t)month);
	f_mkdir(string);	// Create folder if it do not exist
	sprintf( (char *)&string+13 ,"/Day_%02d" , (int16_t)day);
	f_mkdir(string);	// Create folder if it do not exist
	
	// define file name (example "PM_2015_12_05_Event_15_05_30.txt" for 15:05:30 of 5 of December of 2015). File name is added to folder name so pointer move 20 chars.
	sprintf( (char *)&string+20 ,"/LICPM2_%4d_%02d_%02d_Event_%02d_%02d_%02d.txt" , (int16_t)year , (int16_t)month , (int16_t)day , event_det->time.hour , event_det->time.minute , event_det->time.second );
		
	// TODO: Por alguna razon que no entiendo sprintf sobrescribe la variable day. Repito la lectura del RTC hasta entender este problema.
	// get date from RTC
	rtc_get_date(RTC, &year, &month, &day, 0);

	// create file
	if (FR_OK != f_open( &file_object , (char const *)&string , FA_CREATE_ALWAYS | FA_WRITE )) return 0;

	// print device information
	if (EOF == print_device_identification()) return 0;

	// print specific information
	if (EOF == f_printf( &file_object, "Event waveform\n" )) return 0;
	if (EOF == f_puts( "Type = " , &file_object ))return 0;
	switch (event_det->type & ~EVENT_TYPE_BUFFER_USED_MASK) {
		case EVENT_TYPE_RMS_OV_V1_MASK:		if (EOF == f_puts( "V1 Over voltage"  , &file_object )) return 0; break;
		case EVENT_TYPE_RMS_OV_V2_MASK:		if (EOF == f_puts( "V2 Over voltage"  , &file_object )) return 0; break;
		case EVENT_TYPE_RMS_OV_V3_MASK:		if (EOF == f_puts( "V3 Over voltage"  , &file_object )) return 0; break;
		case EVENT_TYPE_RMS_UV_V1_MASK:		if (EOF == f_puts( "V1 Under voltage" , &file_object )) return 0; break;
		case EVENT_TYPE_RMS_UV_V2_MASK:		if (EOF == f_puts( "V2 Under voltage" , &file_object )) return 0; break;
		case EVENT_TYPE_RMS_UV_V3_MASK:		if (EOF == f_puts( "V3 Under voltage" , &file_object )) return 0; break;
		case EVENT_TYPE_RMS_OC_I1_MASK:		if (EOF == f_puts( "I1 Over current"  , &file_object )) return 0; break;
		case EVENT_TYPE_RMS_OC_I2_MASK:		if (EOF == f_puts( "I2 Over current"  , &file_object )) return 0; break;
		case EVENT_TYPE_RMS_OC_I3_MASK:		if (EOF == f_puts( "I3 Over current"  , &file_object )) return 0; break;
		case EVENT_TYPE_FREQ_UF_MASK:		if (EOF == f_puts( "Under frequency"  , &file_object )) return 0; break;
		case EVENT_TYPE_FREQ_OF_MASK:		if (EOF == f_puts( "Over frequency"   , &file_object )) return 0; break;
		default:							if (EOF == f_puts( "Unknown"          , &file_object )) return 0; break;
	}
	// event data
	{
		float temp = event_det->duration_dsp_ticks * (1.0 / SETTINGS_DSP_SYSCLK_HZ);
		if (EOF == f_printf( &file_object, "\nDuration = %d.%03d sec", (int16_t)temp , ABS((int16_t)((temp-(int32_t)temp)*1000.0)) ))return 0;
	}
	if (EOF == f_puts( "\nVoltage Max = " , &file_object )) return 0;
	if (EOF == save_data_x1( &event_det->v_max , gain_voltage )) return 0;
	if (EOF == f_puts( " V\nVoltage Min = " , &file_object )) return 0;
	if (EOF == save_data_x1( &event_det->v_min , gain_voltage )) return 0;
	if (EOF == f_puts( " V\nCurrent Max = " , &file_object )) return 0;
	if (EOF == save_data_x1( &event_det->i_max , gain_current )) return 0;
	if (EOF == f_puts( " A\n" , &file_object)) return 0;
	// time
	if (EOF == f_printf( &file_object, "Trigger Date = %4d/%02d/%02d\n", (int16_t)year , (int16_t)month , (int16_t)day )) return 0;
	if (EOF == f_printf( &file_object, "Trigger Time = %02d:%02d:%02d\n", event_det->time.hour , event_det->time.minute , event_det->time.second )) return 0;
	
	// if buffer were ready to save event waveform, save name
	if (event_det->type & EVENT_TYPE_BUFFER_USED_MASK) {
		copy_content( (uint8_t *)&string[0] ,  (uint8_t *)&string_event_file_name[0] , sizeof(string) );
	}
	
	// Close file
	if (FR_OK != f_close( &file_object )) return 0;
		
	return 1;
	
}

/// @brief Save event into SD-Card.
bool sdcard_save_event_buffer( uint16_t * info , int16_t * buffer) {
	
	// open file or create it if it do not exist.
	if (FR_OK != f_open( &file_object , (char const *)string_event_file_name , FA_OPEN_ALWAYS | FA_WRITE )) return 0;
	
	// Move to end of the file to append data (needed when a existing file is open)
	if (FR_OK != f_lseek( &file_object , f_size(&file_object)) ) return 0;
	
	// Print buffer info if pointer to buffer is NULL.
	if (buffer == NULL ) {
		
		// print index from event info
		if (EOF == f_printf( &file_object , "\nStart index = %d\n" , info[0] )) return 0;
		if (EOF == f_printf( &file_object , "Stop index = %d\n" , info[1] )) return 0;
		if (info[2] == 1)
			{if (EOF == f_puts( "Buffer divided\n\n" , &file_object )) return 0;}
		else
			{if (EOF == f_puts( "Buffer not divided\n\n" , &file_object )) return 0;}
		if (EOF == f_puts( "Counter,V1[V],V2[V],V3[V],VE[V],I1[A],I2[A],I3[A],IN[A]\n" , &file_object )) return 0;
		
	}
	// Else print buffer data to SD-Card
	else {
		
		uint16_t count_channel; // counter of event channel
		int16_t data_f;
		int16_t data_integer;
		int32_t data_fractional;

		// print counter
		
		if (EOF == f_printf( &file_object, "%d,", buffer[0] )) return 0;
		
		// move between event columns
		for ( count_channel=1 ; count_channel<SETTINGS_EVENT_BUFFER_NUM ; count_channel++ ) {
			
			// save data
			data_f = buffer[count_channel];

			// depending on data position, it could be a voltage or current channel. Save absolute value to avoid a ceil rounding for negative values
			if (count_channel<5) {
				data_integer = FPMUL16( abs(data_f) , unit_config.gain_volt , 15 );
				data_fractional = FPMUL32( abs(data_f) , unit_config.gain_volt*100 , 15 ) - (((int32_t)data_integer)*100);
			}
			else {
				data_integer = FPMUL16( abs(data_f) , unit_config.gain_curr_A , 15 );
				data_fractional = FPMUL32( abs(data_f) , unit_config.gain_curr_A*100 , 15 ) - (((int32_t)data_integer)*100);
			}
			
			// Since data is an absolute value, add signal
			if (data_f<0)
				{if (EOF == f_printf( &file_object, "%d.%02d,", -data_integer , data_fractional )) return 0;}
			else
				{if (EOF == f_printf( &file_object, "%d.%02d,",  data_integer , data_fractional )) return 0;}

		}
		if (EOF == f_puts( "\n" , &file_object )) return 0;
		
	}
	
	// Close file
	if (FR_OK != f_close( &file_object )) return 0;
	
	return 1;
}

/*****************************************************************************/

/// @brief Find the older directory (by name).
///
/// @param[in,out] dir_name Pointer to string where name is going to be returned
///
void sdcard_find_older_directory( char * dir_name ) {
	
	static uint8_t i = 0;
	static uint8_t temp_length = 0;

	// set initial value by setting the first characters with 0xFF.
	dir_name[0] = 0xFF;
	
	// infinite loop
	while (1) {
		
		// clear attribute register before update. This register is going to be used to detect the end of FAT table.
		sdcard_file_info.fattrib = 0;
		
		// read next item
		if (FR_OK != f_readdir ( (DIR *)&sdcard_directory , &sdcard_file_info ))
			break;

		// if item is an archive, continue with loop
		if ( sdcard_file_info.fattrib & AM_ARC )
			continue;
		// if item is an directory, check if an older one
		else if ( sdcard_file_info.fattrib & AM_DIR ) {
			// initialize temporal string
			memset( (void *)&string , 0 , sizeof(string) );
			temp_length = sdcard_file_get_name( &sdcard_directory, &sdcard_file_info , (uint8_t *)&string[0] , STRING_MAX );
			
			// if item is "." or ".." continue with the loop
			if (string[0] == 46 ) // 46 -> "."
				continue;		
			
			// read char by char and found the olders
			for ( i = 0 ; i < STRING_MAX ; i++) {
				
				// if string is mayor than dir_name, break for and continue with while
				if ( string[i] > dir_name[i] )
					break;
				// if string is minor than dir_name, update dir_name
				else if ( string[i] < dir_name[i] ) {
					copy_content( (uint8_t *)&string[0] ,
								  (uint8_t *)&dir_name[0] ,
								  temp_length );
				}
				
			}
			
		}
		// if it is not a archive or directory, return since all items were analyzed.
		else {		
			break;
		}
		
	}
	
}

/// @brief Find the older month (by name).
///
/// @param[in,out] dir_name Pointer to array where name of older month is stored
/// @return @e Length of script with name of older directory.
///
uint8_t sdcard_find_older_month(char * dir_name) {

	volatile uint8_t temp_length;

	// define root
	memset( (void *)dir_name , 0 , STRING_MAX );
	sprintf( (char *)dir_name ,"%s" , "0:/");
		
	// open root
	if (FR_OK != f_opendir ( (DIR *)&sdcard_directory , &dir_name[0] ))
		return 0; // Go out since there was a problem.

	// find older year and add name to string
	temp_length = calculate_string_length( (uint8_t *)&dir_name[0] , STRING_MAX );
	sdcard_find_older_directory( &dir_name[temp_length] );

	// open year directory
	if (FR_OK != f_opendir ( (DIR *)&sdcard_directory , &dir_name[0] ))
		return 0; // Go out since there was a problem.

	// find older month
	temp_length = calculate_string_length( (uint8_t *)&dir_name[0] , STRING_MAX );
	dir_name[temp_length] = 47; // 47->"/"
	sdcard_find_older_directory( &dir_name[temp_length+1] );
		
	// calculate new length
	temp_length = calculate_string_length( (uint8_t *)&dir_name[0] , STRING_MAX );
	
	// if last data is 0xFF, year directory was empty
	if (dir_name[temp_length-1] == 0xFF ) {
		dir_name[temp_length-1] = 0;   // clear 0xFF
		dir_name[temp_length-2] = 0; // clear "/"
		return temp_length-2;
	}
	else
		return temp_length;	
	
}

/// @brief check free space and update directory to be removed (oldest) if it is necessary.
void sdcard_check_free_space_and_update_directory_to_remove(void) {

	int32_t data_MB;
	
	data_MB = sdcard_get_total_mbytes_req() - sdcard_get_free_mbytes_req();
	
	if (data_MB > unit_config.sdcard_memory_max_MB)
		dir_remove_name_length = sdcard_find_older_month(&dir_remove_name[0]);

}

/// @brief Remove direcotry
void sdcard_remove_directory(void) {
	
	static char temp_name[STRING_MAX];
	static uint8_t temp_name_length = 0;
	uint8_t loop_count = 10;
	
	// clear string
	memset( (void *)&temp_name , 0 , sizeof(temp_name) );
	
	// copy name of directory and name length
	copy_content( (uint8_t *)dir_remove_name ,
	              (uint8_t *)temp_name,
	              dir_remove_name_length );
	temp_name_length = dir_remove_name_length;
	
	// try to remove subdirectory or file
	while (loop_count) {
			
		// remove file and directory
		res = f_unlink ( (char const *)&temp_name );
		if (res == FR_OK)
			return;
		else if (res != FR_DENIED)
			break;
		
		// open directory
		if (FR_OK != f_opendir ( (DIR *)&sdcard_directory , (const TCHAR *)&temp_name ) )
			break;
			
		// read first item from directory. X3 since two first element are "." and "..".
		if (FR_OK != f_readdir ( (DIR *)&sdcard_directory , &sdcard_file_info ))
			break;
		if (FR_OK != f_readdir ( (DIR *)&sdcard_directory , &sdcard_file_info ))
			break;
		if (FR_OK != f_readdir ( (DIR *)&sdcard_directory , &sdcard_file_info ))
			break;
		
		// agregar "/" (0x2F)
		temp_name[temp_name_length] = 0x2F;
		temp_name_length += 1;
		
		// update name
		temp_name_length += sdcard_file_get_name(  (DIR *)&sdcard_directory, &sdcard_file_info , (uint8_t *)&temp_name[temp_name_length] , STRING_MAX - temp_name_length );

		// decrese counter
		loop_count--;

	}

	// if this line was reached, there was a problem and so clear variable. 
	dir_remove_name_length = 0;

}

/**************************** GLOBAL FUNCTIONS *******************************/


/// @brief SD-Card background routines.
/// @details Check flags for pending task and check if SD-Card is present.
void sdcard_background(void) {

	// update sd-card state
	{
			
		// check SD-Card status	
		status = sd_mmc_test_unit_ready(0);
	
		// if SD-Card enable flag is set, do pending actions.
		if (sdcard_flags_status & SDCARD_FLAGS_STATUS_ENABLE_SDCARD ) {
	
			// if sd-card is not detected yet, check if it is present 
			if (!(sdcard_flags_status & SDCARD_FLAGS_STATUS_DETECT_SDCARD )) {
				if (CTRL_GOOD == status)
					sdcard_flags_status |= SDCARD_FLAGS_STATUS_DETECT_SDCARD;
			}
			// ones sd-card is detected, mount logical driver
			else if	(!(sdcard_flags_status & SDCARD_FLAGS_STATUS_LOGIC_MOUNT)) {
				if (FR_OK == sdcard_mount_logic_drive())
					sdcard_flags_status |= SDCARD_FLAGS_STATUS_LOGIC_MOUNT;
			}
			// ones logical driver is mounted, read unit configuration general parameters.
			else if	(!(sdcard_flags_status & SDCARD_FLAGS_STATUS_READ_UNIT_CONFIG)) {
				if (sdcard_check_and_read_unit_config())
					sdcard_flags_status |= SDCARD_FLAGS_STATUS_READ_UNIT_CONFIG;
			}
			// ones general parameters is read, read calibration parameters and force the send of unit_config and unit_calibration to DSP.
			else if	(!(sdcard_flags_status & SDCARD_FLAGS_STATUS_READ_UNIT_CALIBRATION)) {
				if (sdcard_check_and_read_unit_calibration()) {
					sdcard_flags_status |= SDCARD_FLAGS_STATUS_READ_UNIT_CALIBRATION;
					comm_flags |= SETTINGS_COMM_FLAG_UNIT_CONFIG_REQUEST_MASK | SETTINGS_COMM_FLAG_UNIT_CALIBRATION_REQUEST_MASK;
				}
			}	
			// if everything was done, check if sd-card is still present.
			// If it is, set ready flag.
			else {	
				if (CTRL_GOOD == status)
					sdcard_flags_status |= SDCARD_FLAGS_STATUS_CARD_READY;
				else
					sdcard_flags_status &= 0x1; // only save fist flag
			}
	
		}
		else {
			// clear all flags since Sd-Card is not enable to be used
			sdcard_flags_status = 0;
		}

		// update led	
		if (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY)
			led_green_on();
		else
			led_green_off();
	
	}
	
	// perform sd-card task if sd-card is ready
	if (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY) {
		
		// if task flag for storing of 150 cycle trend is set, do it.
		if (sdcard_flags_task & SDCARD_FLAGS_TASK_SAVE_TRENDS_150CYCLES ) {
			// If something is wrong, do not clear flag.
			if ( 1 == sdcard_save_trends( (trend_t *)&sdcard_trend_150cycles , 0 ) )
				sdcard_flags_task &= ~SDCARD_FLAGS_TASK_SAVE_TRENDS_150CYCLES;
		}
		// if task flag for storing of 5 minutes trend is set, do it.
		else if (sdcard_flags_task & SDCARD_FLAGS_TASK_SAVE_TRENDS_5MINUTES ) {
			// If something is wrong, do not clear flag.
			if ( 1 == sdcard_save_trends( (trend_t *)&sdcard_trend_5minutes , 1 ) )
				sdcard_flags_task &= ~SDCARD_FLAGS_TASK_SAVE_TRENDS_5MINUTES;
		}
		// if task flag for storing event detection info is set, do it.
		else if (sdcard_flags_task & SDCARD_FLAGS_TASK_SAVE_EVENT_DET_INFO ) {
			// If something is wrong, do not clear flag.
			if ( 1 == sdcard_save_event_det_info( (event_det_t *)&sdcard_event_det ) )
				sdcard_flags_task &= ~SDCARD_FLAGS_TASK_SAVE_EVENT_DET_INFO;
		}
		// if task flag for storing buffer info is set, do it.
		else if (sdcard_flags_task & SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER_INFO ) {
			// If something is wrong, do not clear flag.
			if ( 1 == sdcard_save_event_buffer( (uint16_t *)&sdcard_event_buffer_info[0] , NULL ) )
				sdcard_flags_task &= ~SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER_INFO;
		}
		// if task flag for storing buffer data is set, do it.
		else if (sdcard_flags_task & SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER ) {
			// If something is wrong, do not clear flag.
			if ( 1 == sdcard_save_event_buffer( NULL , (int16_t *)&sdcard_event_buffer[0] ) )
				sdcard_flags_task &= ~SDCARD_FLAGS_TASK_SAVE_EVENT_BUFFER;
		}
		
		// if an archive or directory must be removed, do it
		if (dir_remove_name_length)
			sdcard_remove_directory();
		// check if sd-card memory reach limit, if so, oldest directory in root is set to be removed later.
		else if (unit_config.mode_flags & UNIT_CONFIG_MODE_SDCARD_DISCARD_ENABLE)
			sdcard_check_free_space_and_update_directory_to_remove();
		
			
	}
}

/// @brief Save unit configuration into SD-Card.
bool sdcard_save_unit_config(void) {
		
	// Since file do not exist or need to be over written, create it
	if (FR_OK != f_open( &file_object , (char const *)file_name_conf , FA_CREATE_ALWAYS | FA_WRITE )) return 0;
			
	// Save default unit config
	for ( uint8_t i=0 ; i<(sizeof(unit_config)>>2) ; i++ ) {
		f_printf( &file_object , "%d\n", ((int32_t *)&unit_config)[i] );
	}
			
	// Close file
	if (FR_OK != f_close( &file_object )) return 0;
		
	return 1;
	
}

/// @brief Save unit calibration into SD-Card.
bool sdcard_save_unit_calibration(void) {
	
	// Since file do not exist or need to be over written, create it
	if (FR_OK != f_open( &file_object , (char const *)file_name_cal , FA_CREATE_ALWAYS | FA_WRITE )) return 0;
	
	// Save default unit config
	for ( uint8_t i=0 ; i<(sizeof(unit_calibration)>>2) ; i++ ) {
		f_printf( &file_object , "%d\n", ((int32_t *)&unit_calibration)[i] );
	}
	
	// Close file
	if (FR_OK != f_close( &file_object )) return 0;
	
	return 1;
	
}

#if SETTINGS_ARM_ENABLE_SDCARD_COMM_FLAG_SAVE

bool sdcard_save_flags(uint16_t flags) {
	
//	static uint8_t init = 1;
	static FIL file_log;
	uint32_t hour, minute, second;
	uint32_t year, month, day;
		
	// get date from RTC
	rtc_get_time(RTC, &hour, &minute, &second);
	rtc_get_date(RTC, &year, &month, &day, 0);	
	
	// Define directory name (example "2015/Month_12/Day_01" for 1 of December of 2015) and create it.
	memset( (void *)&string , 0 , sizeof(string) );
	sprintf( (char *)&string ,"%4d" , (int16_t)year);
	f_mkdir(string);	// Create folder if it do not exist
	sprintf( (char *)&string+4 ,"/Month_%02d" , (int16_t)month);
	f_mkdir(string);	// Create folder if it do not exist
	sprintf( (char *)&string+13 ,"/Day_%02d" , (int16_t)day);
	f_mkdir(string);	// Create folder if it do not exist
	
	// define file name (example "LICPM2_2015_12_05_log.txt" for 5 of December of 2015). File name is added to folder name so pointer is moved 20 chars.
	sprintf( (char *)&string+20 ,"/LICPM2_%4d_%02d_%02d_log.txt" , (int16_t)year , (int16_t)month , (int16_t)day);
		
	// get date from RTC. TODO: Por alguna razon que no entiendo sprintf sobrescribe la variable day. Repito la lectura del RTC hasta entender este problema.
	rtc_get_date(RTC, &year, &month, &day, 0);

	// open file or create it if it do not exist.
	if (FR_OK != f_open( &file_log , (char const *)string , FA_OPEN_ALWAYS | FA_WRITE ))	return 0;
		
	// Move to end of the file to append data (needed when a existing file is open)
	if (FR_OK != f_lseek( &file_log , f_size(&file_log)) ) return 0;
		
	// save time
	//	if (EOF == f_printf( &file_log, "%4d/%02d/%02d ", (int16_t)year , (int16_t)month , (int16_t)day )) return 0;
	if (EOF == f_printf( &file_log, "%02d:%02d:%02d ", (int16_t)hour , (int16_t)minute , (int16_t)second )) return 0;

	// save flags
	if (EOF == f_printf( &file_log, "%016b ", flags )) return 0;
	if (EOF == f_printf( &file_log, "%08b \n", sdcard_flags_status )) return 0;
		
	// Close file
	if (FR_OK != f_close( &file_log )) return 0;
	
	return 1;
	
}

#endif

/// @brief Get total Mbytes of sd-card.
///
/// @param[out] Total Mbyte
///
uint32_t sdcard_get_total_mbytes_req(void) {

	// Get total bytes (total_MB = clusters * sectors/custers * bytes/sectors / 1024 / 1024)
#if _MAX_SS != 512
	return (( ((uint64_t)fs.n_fatent - 2) * fs.csize * fs.ssize ) / 1048576 );
#else
	return (( ((uint64_t)fs.n_fatent - 2) * fs.csize * 512 ) / 1048576 );
#endif

}

/// @brief Get free Mbytes of sd-card.
///
/// @param[out] Free Mbyte
///
uint32_t sdcard_get_free_mbytes_req(void) {

	DWORD Free_clust;
	FATFS *fs_ptr = &fs;
	
	// Get volume information and free clusters of drive
	res = f_getfree("0:/", &Free_clust, &fs_ptr);

	// return free space
#if _MAX_SS != 512
	return (( ((uint64_t)Free_clust) * fs.csize * fs.ssize ) / 1048576 );
#else
	return (( ((uint64_t)Free_clust) * fs.csize * 512 ) / 1048576 );
#endif	

}

/// @brief Update file and directory name to be removed.
uint8_t sdcard_update_file_and_dir_to_remove( uint8_t * str_name ) {
		
	uint8_t str_length;
	
	// calculate string length	
	str_length = calculate_string_length( str_name , STRING_MAX );
	
	// check if there is a file or directory to be removed
	if (dir_remove_name_length == 0) {
				
		// predefine device root directory
		memset( (void *)&dir_remove_name , 0 , sizeof(dir_remove_name) );
		sprintf( (char *)&dir_remove_name ,"%s" , "0:/");
		
		// copy name of file or directory
		copy_content( str_name ,
			          (uint8_t *)(dir_remove_name + 3),
				      str_length );

		// Verify that name corresponded to a valid file or directory
		if (FR_OK != f_stat ( (char const *)&dir_remove_name , &sdcard_file_info ))
			return 1;
		
		// All ok -> update length
		dir_remove_name_length = str_length+3;
		
		// return
		return 0;
		
	}
	else
		return 2;
	
}

/// @brief Get file/folder name.
///
/// @param[inout] directory
/// @param[inout] file_info
/// @param[inout] name_string
/// @param[out] name_length
///
/// @note string_name must be cleared outside.
/// @note It must be called after next item function since other fatfs function 
///       related to sd-card could modified the buffer where long name is saved.
uint8_t sdcard_file_get_name(DIR * directory, FILINFO * file_info , uint8_t * name_string , uint8_t limit) {

	uint8_t name_length = 0;

	// check if there is a short file name
	if  (directory->lfn_idx == 0xFFFF) {

		// calculate length
		name_length = calculate_string_length( (uint8_t *)&file_info->fname[0] , 13 );
		
		// copy name of file
		copy_content( (uint8_t *)&(file_info->fname[0]) ,
			   		  name_string,
				      name_length ); 
	}
	// it is a long file name (LFN)
	else {
		
		// LFN is stored in a string of 16bits word, so a custom code is used
		for (;;) {
			if ((*(directory->lfn + name_length) != 0) & (name_length < limit)) {
				name_string[name_length] = *(directory->lfn + name_length);
				name_length++;
			}
			else
				break;
		}

	}
	
	return name_length;

}


/// @}




  




















