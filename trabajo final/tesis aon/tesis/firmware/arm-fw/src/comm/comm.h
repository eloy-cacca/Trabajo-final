/** $Header
 * @file           comm.h
 * @brief          Communications function.
 * @details
 * @author         Agustin Aon
 * @version        0.1
 * @date           21 July 2020
 * @details
 * @copyright
 */


#ifndef __COMM_H__
#define __COMM_H__

/**************************** INCLUDES ***************************************/
#include "settings.h"


/**************************** MACROS *****************************************/

// SCI
#define UART_PC             (UART1)
#define USB_PC                (NULL)

// Parameters
#define MSG_LENGTH_HEADER   ( 8 )   // 1 Start + 1 Proj. ID + 2 Dev. ID + 1 Msg. Id + 1 payload length + 2 CRC
#define MSG_LENGTH_PAYLOAD  ( 128 )
#define MSG_LENGTH_MAX_SIZE ( MSG_LENGTH_HEADER + MSG_LENGTH_PAYLOAD )


/**************************** ENUMS, STRUCTS *********************************/

/** SCI structure */
typedef struct {
    Uart     *uart_ptr;                      /**< pointer to UART port */
    uint8_t  rx_counter;                     /**< counter of received data */
    uint32_t timeout_counter;                /**< timeout for clear RX buffer */
    uint8_t  packet_in[MSG_LENGTH_MAX_SIZE]; /**< RX buffer */
} msg_t;

/** Message bytes order */
enum msg_in_header_t {
    msg_h_start=0,
    msg_h_proj_id,
    msg_h_src_id,
    msg_h_des_id,
    msg_h_msg_id,
    msg_h_payload_len,
    msg_h_payload_1st_byte
};


/**************************** FUNCTIONS **************************************/
void comm_init(void);
void comm_background(void);

void comm_preprocess_uart_PC_RX(void);
void comm_preprocess_usb_PC_RX(void);

void comm_send_msg(Uart *uart_ptr , uint8_t msg_des, uint8_t msg_id , uint8_t payload_len , uint8_t * payload_ptr);


#endif /* __COMM_H__ */