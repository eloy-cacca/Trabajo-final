// $Header
/// @file			sdcard_api.h
/// @brief  		Firmware application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		02 June 2017
/// @details
/// @copyright
///

/// @addtogroup apis
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "sdcard_api.h"

// local includes
#include "../comm/app_layer.h"
#include "../apps/sdcard.h"
#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

#define SDCARD_FILE_READ_LENGTH     sizeof(UINT)
#define SDCARD_FILE_READ_BYTE_MAX	(SETTINGS_MSG_LENGHT_PAYLOAD - SDCARD_FILE_READ_LENGTH)

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

FRESULT res;
DIR directory;
FILINFO file_info;
FIL sdcard_api_file_object;

uint8_t sdcard_api_string[128];

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Response to open directory.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_SDCARD_dir_open_req( msg_t * msg_ptr )
{

	// predefine device root directory
	memset( (void *)&sdcard_api_string , 0 , sizeof(sdcard_api_string) );
	sprintf( (char *)&sdcard_api_string ,"%s" , "0:/"); // store first three characteres
	
	// copy name of directory
	copy_content( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
	              ((uint8_t *)&sdcard_api_string) + 3,
	              msg_ptr->packet_in[msg_h_payload_len] );
	
	// open directory
	res = f_opendir ( (DIR *)&directory , (const TCHAR *)&sdcard_api_string );
	
	// Send response
	comm_send_msg( msg_ptr->uart_ptr ,
		           msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               sizeof(FRESULT),
	               (uint8_t *)&res);

}

/// @brief Response to read item.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_SDCARD_item_next_req( msg_t * msg_ptr )
{

	uint8_t payload_len;
	uint8_t * payload_ptr;
		
	// Accept command only if payload length is equal to 0
	if (msg_ptr->packet_in[msg_h_payload_len] == 0) {
		
		// clear previous file info
		memset( (void *)&file_info , 0 , sizeof(FILINFO) );
	
		// read item from directory
		res = f_readdir ( (DIR *)&directory , &file_info );

		// Define response depending on f_opendir.
		if (res == FR_OK) {
			
			// clear string
			memset( (void *)&sdcard_api_string , 0 , sizeof(sdcard_api_string) );
					
			// Get file name.
			// NOTE: Long file name (LFN) is stored in a register used by Fat file system. Other
			// function from fatfs library (that could be executed in background) are going to 
			// overwrite the data and the LFN are going to be erased. As result, file name is
			// stored here in the general purpose string and it must be request next.
			sdcard_file_get_name( &directory, &file_info , &sdcard_api_string[0] , STRING_MAX);
			
			// response
			payload_len = sizeof(FILINFO);
			payload_ptr = (uint8_t *)&file_info;
		}
		else {
			payload_len = sizeof(FRESULT);
			payload_ptr = (uint8_t *)&res;
		}

		// Send response
		comm_send_msg( msg_ptr->uart_ptr ,
					   msg_ptr->packet_in[msg_h_src_id] ,
					   msg_ptr->packet_in[msg_h_msg_id]+1 ,
					   payload_len ,
					   payload_ptr);

	}

}

/// @brief Response to read name item.
///
/// @param[in] msp_ptr Pointer to incoming message
///
/// @note File/directory name must be stored previously.
void API_SDCARD_item_name_req( msg_t * msg_ptr )
{

	uint8_t name_length = 0;
	
	// Accept command only if payload length is equal to 0
	if (msg_ptr->packet_in[msg_h_payload_len] == 0) {
			
		// calculate LFN length (find null caracter)
		for (;;) {
			if (sdcard_api_string[name_length] != 0)
				name_length++;
			else
				break;
		}
			
		// Send response
		comm_send_msg( msg_ptr->uart_ptr ,
		               msg_ptr->packet_in[msg_h_src_id] ,
		               msg_ptr->packet_in[msg_h_msg_id]+1 ,
		               name_length,
		               (uint8_t *)&sdcard_api_string[0]);

	}

}

/// @brief Response to file size request.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_SDCARD_file_size_req( msg_t * msg_ptr ) {

	// Accept command only if payload length is equal to 0
	if (msg_ptr->packet_in[msg_h_payload_len] == 0) {
	
		// Send response
		comm_send_msg( msg_ptr->uart_ptr ,
					   msg_ptr->packet_in[msg_h_src_id] ,
					   msg_ptr->packet_in[msg_h_msg_id]+1 ,
					   sizeof(DWORD),
					   (uint8_t *)&sdcard_api_file_object.fsize);
	
	}
}

/// @brief Response to open file request.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_SDCARD_file_open_req( msg_t * msg_ptr ) {
	
	// predefine device root directory
	memset( (void *)&sdcard_api_string , 0 , sizeof(sdcard_api_string) );
	sprintf( (char *)&sdcard_api_string ,"%s" , "0:/");
		
	// copy name of directory
	copy_content( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
				  ((uint8_t *)&sdcard_api_string) + 3,
				  msg_ptr->packet_in[msg_h_payload_len] );
		
	// open directory
	res = f_open( &sdcard_api_file_object , (char const *)&sdcard_api_string , FA_READ );
		
	// Send response
	comm_send_msg( msg_ptr->uart_ptr ,
	               msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               sizeof(FRESULT),
	               (uint8_t *)&res);
	
}

/// @brief Response to read file request.
/// @Note It send 127 byte by request. 
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_SDCARD_file_read_req( msg_t * msg_ptr ) {
	
	UINT br;
	UINT offset;
	
	// Accept command only if payload length is equal to SDCARD_FILE_READ_LENGTH
	if (msg_ptr->packet_in[msg_h_payload_len] == SDCARD_FILE_READ_LENGTH) {
	
		// Read offset
		copy_content( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] ,
		              (uint8_t *)&offset,
		               SDCARD_FILE_READ_LENGTH );
		
		// Moves the file read/write pointer
		res = f_lseek ( &sdcard_api_file_object , offset*SDCARD_FILE_READ_BYTE_MAX );

		// Reads data from a file. Left SDCARD_FILE_READ_LENGTH bytes to store number of read bytes.
		res = f_read ( &sdcard_api_file_object , (uint8_t *)&sdcard_api_string[SDCARD_FILE_READ_LENGTH] , SDCARD_FILE_READ_BYTE_MAX , &br );
		
		// save read bytes
		copy_content( (uint8_t *)&br ,
		              (uint8_t *)&sdcard_api_string,
		              SDCARD_FILE_READ_LENGTH );

		// Send response
		comm_send_msg( msg_ptr->uart_ptr ,
		               msg_ptr->packet_in[msg_h_src_id] ,
		               msg_ptr->packet_in[msg_h_msg_id]+1 ,
		               br + SDCARD_FILE_READ_LENGTH,
		               (uint8_t *)&sdcard_api_string);
		

	}
	
}

/// @brief Response to close file request.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_SDCARD_file_close_req( msg_t * msg_ptr ) {
	
	uint8_t response;
	
	// Accept command only if payload length is equal to 0
	if (msg_ptr->packet_in[msg_h_payload_len] == 0) {

		res = f_close( &sdcard_api_file_object );

		// response
		response = res;
			
	}
	else {
		
		// response
		response = 0xFF;

	}

	// Send response
	comm_send_msg( msg_ptr->uart_ptr ,
	               msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               1 ,
	               &response);
	
}


/// @brief Response to remove file or directory.
///
/// @param[in] msp_ptr Pointer to incoming message
///
/// @note Removing a directory could take a long time since all files and sub-directory
///       must be removed first. As, result, this function update the file/directory to
///       be removed in background (one file each time that it is executed).
void API_SDCARD_file_and_dir_remove_req( msg_t * msg_ptr ) {
	
	uint8_t response;
				  
	// update folder to be removed
	response = sdcard_update_file_and_dir_to_remove( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] );
			
	// Send response
	comm_send_msg( msg_ptr->uart_ptr ,
	               msg_ptr->packet_in[msg_h_src_id] ,
				   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               1 ,
	               &response);
	
}
	

/// @brief Response to size request.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_SDCARD_get_mbytes_req( msg_t * msg_ptr ) {

	uint32_t info[2];
	
	info[0] = sdcard_get_total_mbytes_req();
	info[1] = sdcard_get_free_mbytes_req();

	// Send response
	comm_send_msg( msg_ptr->uart_ptr ,
				   msg_ptr->packet_in[msg_h_src_id] ,
	   			   msg_ptr->packet_in[msg_h_msg_id]+1 ,
				   sizeof(info),
				   (uint8_t *)&info);
	
	
}

/// @}

