// $Header
/// @file			comm.h
/// @brief  		Communication file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		27 October 2016
/// @details
/// @copyright
///

#ifndef __COMM_H__
#define __COMM_H__

/**************************** INCLUDES ***************************************/

// Specific includes
#include "settings.h"

// Local includes


/**************************** MACROS *****************************************/

// SCI
#define SCIA                (SciaRegs)
#define SCIB                (ScibRegs)
#define SCIC                (ScicRegs)

/**************************** ENUMS, STRUCTS *********************************/

// SCI structure
typedef struct {
	sci_reg_t *sci_ptr;                // pointers to sci port
	uint8_t  rx_counter;               // counter of received data
	uint32_t timeout_counter;          // timeout for clear RX buffer
	uint8_t  packet_in[SETTINGS_MSG_LENGHT_MAX_SIZE];  // RX buffer
} msg_t;

// Message bytes
enum msg_in_header_t {
    msg_h_start=0,
    msg_h_proj_id,
	msg_h_src_id,
	msg_h_des_id,
	msg_h_msg_id,
	msg_h_payload_len,
	msg_h_payload_1st_byte
};

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

void comm_init(void);
void comm_background(void);

void comm_sci_a_rx(void);
void comm_sci_b_rx(void);
void comm_sci_c_rx(void);

void comm_send_message( sci_reg_t *uart_ptr , uint8_t msg_des, uint8_t msg_id , uint8_t payload_len , uint8_t * payload_ptr );

/// @endcond

#endif // __COMM_H__

