// $Header
/// @file			bootloader.c
/// @brief  		Bootloader aplication file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		21 December 2017
/// @details
/// @copyright
///

/// @addtogroup template
/// @{

/**************************** INCLUDES ***************************************/

#include "bootloader.h"

#include "../common_DSP_ARM/comm_common.h"
#include "../apps/system.h"

#include "../utils/byte_string_operations.h"

/**************************** MACROS *****************************************/

#define FLASH_SECTOR_SIZE		( 0x00010000 )
#define RUN_BOOT_FLAG_SET		( 0xAAAAu )
#define BOOT_SIZE				( 8 * 1024 )

/**************************** ENUMS, STRUCTS *********************************/

typedef enum
{
	FLASH_SIZE_8K = 1,
	FLASH_SIZE_16K = 2,
	FLASH_SIZE_32K = 3,
	FLASH_SIZE_64K = 5,
	FLASH_SIZE_128K = 7,
	FLASH_SIZE_256K = 9,
	FLASH_SIZE_512K = 10,
	FLASH_SIZE_1024K = 12,
	FLASH_SIZE_2048K = 14,
} flash_size_t;


typedef struct
{
	uint32_t start;
	uint32_t size;
	uint32_t flag_add;
	uint32_t crc_add;
	uint32_t size_add;
} boot_add_t;

/**************************** FUNCTION PROTOTYPES ****************************/

uint32_t scratch_erase(void);
uint8_t start_image_download(uint32_t new_img_size, uint16_t chunks_size);
uint8_t save_image_block(uint16_t blk_number, uint8_t* blk_ptr, uint16_t blk_size);
bool_t verify_scratch_image(uint32_t img_size, uint16_t img_crc);
uint16_t scratch_checksum(uint32_t image_size);
uint32_t boot_set_flag(uint16_t fw_crc, uint32_t fw_size);

/**************************** LOCAL VARIABLES ********************************/

boot_add_t app_temp;
new_image_t new_image;
uint8_t start_upload_ready = 0;

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

static flash_size_t get_flash_size(void)
{
	return (flash_size_t)((CHIPID->CHIPID_CIDR & CHIPID_CIDR_NVPSIZ_Msk) >> CHIPID_CIDR_NVPSIZ_Pos);
}

bool_t verify_scratch_image(uint32_t img_size, uint16_t img_crc)
{
	if (img_size == 0)
		return FALSE;

	if (scratch_checksum(img_size) == img_crc)
		return TRUE;
	
	return FALSE;
}


/// @brief Function to erase the scratchpad space on flash memory
///
/// @return 0 if successful; otherwise returns an error code.
///
uint32_t scratch_erase(void)
{	
	uint8_t i = 0;
	uint32_t erase_address = app_temp.start;
	uint32_t erase_error = 0;
	
	/* Flash Unlock Temp App region */
	erase_error = flash_unlock(app_temp.start, app_temp.start + app_temp.size - 1, 0, 0);
	
	/* Erase sectors from Temp App region */
	for (i=0; i<(app_temp.size / FLASH_SECTOR_SIZE); i++)
	{
		erase_error |= flash_erase_sector(erase_address);
		erase_address += FLASH_SECTOR_SIZE;
	}
	
	/* Flash Lock Temp App region */
	erase_error |= flash_lock(app_temp.start, app_temp.start + app_temp.size - 1, 0, 0);
	
	return erase_error;
}

/// @brief Function used to calculate the scratch-pad image checksum
///
/// @param[in] image_size Image length
/// @return Checksum value is returned in a 16 bits word.
///
uint16_t scratch_checksum(uint32_t image_size)
{
	uint16_t checksum = 0;
	
	calculate_checksum_ptr((uint8_t*)app_temp.start, image_size, &checksum);
		
	return checksum;
}

uint8_t save_image_block(uint16_t blk_number, uint8_t* blk_ptr, uint16_t blk_size)
{
	uint32_t blk_error;
	uint32_t blk_offset = (uint32_t)blk_number * new_image.block_size;
	
	/* Save block into scratch. Only write block if memory content is different */
	if (memcmp((const void *)(blk_ptr), (const void *)(app_temp.start + blk_offset), blk_size))
	{
		/* Disable interrupts */
		__disable_fault_irq();
		/* Flash Unlock */
		blk_error = flash_unlock((uint32_t)app_temp.start + blk_offset, (uint32_t)app_temp.start + blk_offset + blk_size - 1, 0, 0);
		/* Flash Write */
		blk_error |= flash_write((uint32_t)app_temp.start + blk_offset, blk_ptr, (uint32_t)blk_size, 0);
		/* Flash Lock */
		blk_error |= flash_lock((uint32_t)app_temp.start + blk_offset, (uint32_t)app_temp.start + blk_offset + blk_size - 1, 0, 0);
		/* Enable interrupts */
		__enable_fault_irq();
		
		if (blk_error != 0)
		{
			return 1;
		}
	}
		
	return 0;
}

uint32_t boot_set_flag(uint16_t fw_crc, uint32_t fw_size)
{
	uint32_t error_code;
	static uint16_t boot_flag = RUN_BOOT_FLAG_SET;
	uint8_t boot_flag_buffer[8];
	
	/* Save on last flash page boot flag and new image checksum and size  */
	memcpy(boot_flag_buffer, &boot_flag, sizeof(boot_flag));
	memcpy(boot_flag_buffer + 2, &fw_crc, sizeof(fw_crc));
	memcpy(boot_flag_buffer + 4, &fw_size, sizeof(fw_size));
	
	/* Disable interrupts */
	__disable_fault_irq();
	/* Flash Unlock */
	error_code = flash_unlock((uint32_t)app_temp.flag_add, (uint32_t)app_temp.start + app_temp.size - 1, 0, 0);
	/*Flash Write */
	error_code |= flash_write((uint32_t)app_temp.flag_add, boot_flag_buffer, sizeof(boot_flag_buffer), 0);
	/* Flash Lock */
	error_code |= flash_lock((uint32_t)app_temp.flag_add, (uint32_t)app_temp.start + app_temp.size - 1, 0, 0);
	/* Enable interrupts */
	__enable_fault_irq();
	
	return error_code;
}

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Bootloader initialization.
/// @details Scratchpad origin and length are estimated.
void bootloader_init(void){
	
	// Get flash size and critical memory addresses
	switch (get_flash_size())
	{
		case FLASH_SIZE_128K:
			app_temp.start = 0x00410000;
			app_temp.size = 0x00010000;
			break;
			
		case FLASH_SIZE_256K:
			app_temp.start = 0x00420000;
			app_temp.size = 0x00020000;
			break;
			
		case FLASH_SIZE_512K:
			app_temp.start = 0x00440000;
			app_temp.size = 0x00040000;
			break;
			
		case FLASH_SIZE_1024K:
			app_temp.start = 0x00480000;
			app_temp.size = 0x00080000;
		break;
			
		case FLASH_SIZE_2048K:
			app_temp.start = 0x00500000;
			app_temp.size = 0x00100000;
		break;
			
		default:
			app_temp.start = 0x00410000;
			app_temp.size = 0x00010000;
			break;
	}
		
	// Load boot flag, app temp image checksum and size addresses
	app_temp.flag_add = app_temp.start + app_temp.size - 8;
	app_temp.crc_add = app_temp.start + app_temp.size - 6;
	app_temp.size_add = app_temp.start + app_temp.size - 4;
		
}

/// @brief Bootloader start upload of new image.
///
/// @param[in] payload Pointer to input data
/// @return 0 if successful; otherwise returns an error code:
///         2- FW version is incompatible with hardware.
///         3- Image bigger than space available.
///         4- CRC (param) same as CRC image on scratch pad space.
///         5- Blocks size bigger than max payload message
///         6- Fail to erase flash.
///
uint8_t bootloader_start_upload( uint8_t * payload ) {
		
	start_upload_ready = 0;
		
	// Save new image info
   	copy_content( payload , (uint8_t *)&new_image , sizeof(new_image_t) );
	
	// Check new FW version
//	if ( new_image.fw_version < ( (SETTINGS_ARM_FW_VERSION<<8) | SETTINGS_ARM_FW_VERSION_ENG_BUILD )  )
//		return 2;
		
	// Check that new FW fits into application space
	if (new_image.length > (app_temp.size - BOOT_SIZE))
		return 3;
	
	// Check if image on temp app region is the same that is going to be download
//	if (verify_scratch_image(new_image.length, new_image.crc))
//		return 4;
		
	// Check block size
	if (new_image.block_size > (SETTINGS_MSG_LENGHT_PAYLOAD - sizeof(uint16_t) ) )
		return 5;
			
	// Erase scratch-pad
	if (scratch_erase() != 0)
		return 6;
						
	// All is ok
	start_upload_ready = 1;
	return 0;
		
}

/// @brief Bootloader update block.
///
/// @param[in] payload Pointer to input data
/// @return 0 if successful; otherwise returns an error code:
///         2- Start upload command was not successfully executed.
///         3- Fail to save block.
///
uint8_t bootloader_upload_block( uint8_t * payload , uint8_t payload_length ) {
	
	uint16_t blk_number;
		
	// Load block number
	memcpy(&blk_number, payload , sizeof(blk_number));
		
	// Only execute the followings command if start upload command was executed successfully
	if (start_upload_ready == 0)
		return 2;
	
	if (save_image_block(blk_number, payload + 2, payload_length - 2) != 0)
		return 3;
		
	// All is ok
	return 0;
			
}

/// @brief Bootloader install image.
///
/// @param[in] payload Pointer to input data
/// @return 0 if successful; otherwise returns an error code:
///			2- received info do not match with previous info 
///			3- CRC (param) same as CRC image on scratch pad space.
///         4- flags not set
///			5- ARM was not reset
///
uint8_t bootloader_install_image( uint8_t * payload ) {	
		
	// check that info is equal to info received with the start command
	if (compare_content( (uint8_t *)&new_image , payload , sizeof(new_image_t)) != 0)
		return 2;
	
	// Check if CRC received as parameter matches with the scratchpad image CRC
	if (verify_scratch_image(new_image.length, new_image.crc) == FALSE)
		return 3;
	
	// Set flags
	if (boot_set_flag(new_image.crc, new_image.length) != 0)
		return 4;
	
	// clear flag of start command	
	start_upload_ready = 0;
	
	// Reset ARM
	system_reset_ARM();

	// Should never get here
	return 0;
	
}

/// @}

