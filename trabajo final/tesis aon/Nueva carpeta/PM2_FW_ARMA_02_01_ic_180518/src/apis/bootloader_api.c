// $Header
/// @file			bootloader_api.c
/// @brief  		Bootloadr application interface.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		20 December 2017
/// @details
/// @copyright
///

/// @addtogroup apis
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "bootloader_api.h"

// local includes
#include "../apps/bootloader.h"
#include "../comm/app_layer.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Start firmware update.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_BOOT_ARM_start_upload_req( msg_t * msg_ptr )
{
	
	uint8_t response;
	
	// Accept command only if payload length is equal to new image structure
    if ( msg_ptr->packet_in[msg_h_payload_len] == sizeof(new_image_t) ) {

		response = bootloader_start_upload( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] );

	} 
	else {

	    response = 0x1;

    }	
		
	comm_send_msg( msg_ptr->uart_ptr ,
	               msg_ptr->packet_in[msg_h_src_id] ,
	               msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               1 ,
 	               &response);

}

/// @brief Update block in scratchpad.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_BOOT_ARM_upload_block_req( msg_t * msg_ptr )
{

	uint8_t response;
	
	response = bootloader_upload_block( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] , msg_ptr->packet_in[msg_h_payload_len] );
	
	comm_send_msg( msg_ptr->uart_ptr ,
                   msg_ptr->packet_in[msg_h_src_id] ,
                   msg_ptr->packet_in[msg_h_msg_id]+1 ,
                   1 ,
                   &response);
	
}

/// @brief Install image from scratchpad.
///
/// @param[in] msp_ptr Pointer to incoming message
///
void API_BOOT_ARM_install_image_req( msg_t * msg_ptr )
{

	uint8_t response;
	
	// Accept command only if payload length is equal to new image structure
	if ( msg_ptr->packet_in[msg_h_payload_len] == sizeof(new_image_t) ) {

		response = bootloader_install_image( (uint8_t *)&msg_ptr->packet_in[msg_h_payload_1st_byte] );

	}
	else {

		response = 0x1;

	}
	
	comm_send_msg( msg_ptr->uart_ptr ,
               	   msg_ptr->packet_in[msg_h_src_id] ,
               	   msg_ptr->packet_in[msg_h_msg_id]+1 ,
	               1 ,
                   &response);
	
}


/// @}

