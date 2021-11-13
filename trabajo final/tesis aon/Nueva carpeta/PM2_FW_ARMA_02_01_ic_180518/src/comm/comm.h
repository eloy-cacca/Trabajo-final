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
#define UART_RPI            (UART1)
#define UART_DSP            (UART0)

// Parameters
#define MSG_LENGHT_HEADER   ( 8 )   // 1 Start + 1 Proj. ID + 2 Dev. ID + 1 Msg. Id + 1 payload length + 2 CRC
#define MSG_LENGHT_PAYLOAD  ( 128 )
#define MSG_LENGHT_MAX_SIZE ( MSG_LENGHT_HEADER + MSG_LENGHT_PAYLOAD )

/**************************** ENUMS, STRUCTS *********************************/

// SCI structure
typedef struct {
	Uart  *  uart_ptr;                       // pointers to UART port
	uint8_t  rx_counter;                     // counter of received data
	uint32_t timeout_counter;                // timeout for clear RX buffer
	uint8_t  packet_in[MSG_LENGHT_MAX_SIZE]; // RX buffer
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
void comm_preprocess_uart_DSP_RX(void);
void comm_preprocess_uart_RPI_RX(void);

//void comm_sci_a_rx(void);
//void comm_sci_b_rx(void);
//void comm_sci_c_rx(void);

void comm_send_msg( Uart *uart_ptr , uint8_t msg_des, uint8_t msg_id , uint8_t payload_len , uint8_t * payload_ptr );
void comm_bypass_msg( Uart *uart_ptr , msg_t * msg_ptr );

void comm_counters_update(void);
void comm_TX_counter_clear(void);
void comm_TX_counter_set(void);

/// @endcond

#endif // __COMM_H__

