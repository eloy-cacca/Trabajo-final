// $Header
/// @file			bootloader.h
/// @brief  		Bootloader aplication file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		21 December 2017
/// @details
/// @copyright
///

#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

typedef struct {

	uint32_t fw_version;
	uint32_t length;
	uint32_t crc;
	uint32_t block_size;

} new_image_t;

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void bootloader_init(void);

uint8_t bootloader_start_upload( uint8_t * payload );
uint8_t bootloader_upload_block( uint8_t * payload , uint8_t payload_length);
uint8_t bootloader_install_image( uint8_t * payload );

/// @endcond

#endif // __BOOTLOADER_H__

