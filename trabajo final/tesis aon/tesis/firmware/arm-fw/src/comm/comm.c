/**
 * $Header
 * @file           comm.c
 * @brief          Comm function.
 * @details
 * @author         Ignacio Carugati
 * @version        0.01
 * @date           21 July 2020
 * @details
 * @copyright
 *
 * @addtogroup main
 * @{
 */


/**************************** INCLUDES ***************************************/
// Specific includes
#include "comm.h"

// Local includes
#include "app_layer.h"
#include "../utils/byte_string_operations.h"


/**************************** MACROS *****************************************/
#define MSG_START_BYTE      ((uint8_t)(0x3C))
#define MSG_TIMEOUT         ( 0x000007FF )


/**************************** FUNCTION PROTOTYPES ****************************/
static void comm_preprocess_RX_usb_msg(msg_t * msg_ptr);
static void comm_preprocess_RX_uart_msg(msg_t * msg_ptr);
static void comm_process_RX_uart_msg(msg_t * msg_ptr);
static void comm_process_RX_usb_msg(msg_t * msg_ptr);

static void uart_send_packet(Uart *uart_ptr, uint8_t *tx_packet, uint16_t tx_len);
static void usb_send_packet(void* pack_out, uint32_t length);


/**************************** LOCAL VARIABLES ********************************/

// Sci variables
msg_t UART_PC_msg;
msg_t USB_PC_msg;

// output
uint8_t packet_out[MSG_LENGTH_MAX_SIZE];
bool    packet_out_busy = 0;

// usb specific
uint32_t usb_received_size;


/**************************** LOCAL FUNCTIONS ********************************/

/** @brief Generic USB CDC pre-process incoming message
  * @details This functions is called by Rx interrupt when a byte is received.
  *          Byte is added to buffer and counter of received data is updated.
  * @param[in] msg_ptr Pointer to msg_t
  */
static void comm_preprocess_RX_usb_msg(msg_t *msg_ptr)
{
    // Check if a byte was received
    if (udi_cdc_is_rx_ready())
    {
        // get the number of bytes received
        usb_received_size = udi_cdc_get_nb_received_data();

        // save new byte
        udi_cdc_read_buf((void *)&msg_ptr->packet_in[msg_ptr->rx_counter], usb_received_size);

        // increment the rx counter
        msg_ptr->rx_counter = msg_ptr->rx_counter + usb_received_size;

        // reset timeout counter
        msg_ptr->timeout_counter = 0;
    }
}

/** @brief Generic UART pre-process incoming message
  * @details This functions is called by Rx interrupt when a byte is received.
  *          Byte is added to buffer and counter of received data is updated.
  * @param[in] msg_ptr Pointer to msg_t
  */
static void comm_preprocess_RX_uart_msg(msg_t *msg_ptr)
{
    // Check if a byte was received
    if (uart_is_rx_ready(msg_ptr->uart_ptr))
    {
        // save new byte
        uart_read(msg_ptr->uart_ptr, &msg_ptr->packet_in[msg_ptr->rx_counter]);

        // increment the rx counter
        msg_ptr->rx_counter++;

        // reset timeout counter
        msg_ptr->timeout_counter = 0;
    }
}

/** @brief   Generic UART process incoming message
  * @details This functions is called in background and check if a message is complete.
  * @param[in] msg_ptr Pointer to UART structure
  */
static void comm_process_RX_uart_msg(msg_t * msg_ptr)
{
    // If error occurs, start over.
    if ( uart_get_status(msg_ptr->uart_ptr) == (UART_SR_OVRE | UART_SR_FRAME | UART_SR_PARE) )
    {
        memset(msg_ptr->packet_in, 0, MSG_LENGTH_MAX_SIZE);     // Reset buffer
        msg_ptr->rx_counter = 0;                                // Reset rx index
        msg_ptr->timeout_counter = 0;                           // Reset timeout counter
        uart_reset_status(msg_ptr->uart_ptr);                    // Clear error since it will block receiving
        // TODO: this is a problem for other uC.
    }

    // Increment timeout counter
    msg_ptr->timeout_counter++;

    // Reset counters if timeout occurs
    if (msg_ptr->timeout_counter == MSG_TIMEOUT)
    {
        msg_ptr->timeout_counter = 0;
        msg_ptr->rx_counter = 0;
    }

    // Check proper start byte
    if (msg_ptr->rx_counter > 0)
    {
        if (msg_ptr->packet_in[msg_h_start] != MSG_START_BYTE)
        msg_ptr->rx_counter = 0; // reset buffer
    }

    // Check proper project id
    if (msg_ptr->rx_counter > 1)
    {
        if (!(msg_ptr->packet_in[msg_h_proj_id] == SETTINGS_PROJECT_ID) )
        msg_ptr->rx_counter = 0; // reset buffer
    }

    // Check proper source id
    if (msg_ptr->rx_counter > 2)
    {
        if (!( (msg_ptr->packet_in[msg_h_src_id] == SETTINGS_OTHERS_ID) ||
               (msg_ptr->packet_in[msg_h_src_id] == SETTINGS_PC_ID) ))
            msg_ptr->rx_counter = 0; // reset buffer
    }

    // Check proper destination id
    if (msg_ptr->rx_counter > 3)
    {
        if (!( (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_ARM_ID) ||
               (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_PC_ID) ))
            msg_ptr->rx_counter = 0; // reset buffer
    }

    // if the packet is complete, check CRC and packet out is not busy, decode message and reset buffer
    if (  (msg_ptr->rx_counter >= MSG_LENGTH_HEADER) &&
          ((msg_ptr->packet_in[msg_h_payload_len] + MSG_LENGTH_HEADER) == msg_ptr->rx_counter) &&
          (!packet_out_busy ) &&
          (!calculate_checksum( msg_ptr->packet_in , (uint16_t) msg_ptr->rx_counter)) )
    {
        decode_incoming_comms_packet( msg_ptr );
        msg_ptr->rx_counter = 0; // reset buffer
    }

}

/** @brief        Generic USB process incoming message
  * @details    This functions is called in background and check if a message is complete.
  * @param[in]    msg_ptr Pointer to USB structure
  */
static void comm_process_RX_usb_msg( msg_t * msg_ptr )
{
    // Increment timeout counter
    msg_ptr->timeout_counter++;

    // Reset counters if timeout occurs
    if (msg_ptr->timeout_counter == MSG_TIMEOUT)
    {
        msg_ptr->timeout_counter = 0;
        msg_ptr->rx_counter = 0;
    }

    if (msg_ptr->rx_counter > 0)
    {
        // Check proper bytes
        if (
            // start byte
            (msg_ptr->packet_in[msg_h_start] != MSG_START_BYTE)    ||

            // project id
            (msg_ptr->packet_in[msg_h_proj_id] != SETTINGS_PROJECT_ID) ||

            // source id
            !( (msg_ptr->packet_in[msg_h_src_id] == SETTINGS_OTHERS_ID) ||
               (msg_ptr->packet_in[msg_h_src_id] == SETTINGS_PC_ID)
            ) ||

            // destination id
            !( (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_ARM_ID) ||
               (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_PC_ID)
            )
           )
            msg_ptr->rx_counter = 0; // reset buffer
    }

    // if the packet is complete, check CRC and packet out is not busy, decode message and reset buffer
    if (  (msg_ptr->rx_counter >= MSG_LENGTH_HEADER) &&
          ((msg_ptr->packet_in[msg_h_payload_len] + MSG_LENGTH_HEADER) == msg_ptr->rx_counter) &&
          (!packet_out_busy ) &&
          (!calculate_checksum( msg_ptr->packet_in , (uint16_t) msg_ptr->rx_counter)) )
    {
        decode_incoming_comms_packet( msg_ptr );
        msg_ptr->rx_counter = 0; // reset buffer
    }

}
/**    @brief Sends packet over UART */
static void uart_send_packet(Uart *uart_ptr, uint8_t *tx_packet, uint16_t tx_len)
{
    while (tx_len--)
    {
        // wait for sci FIFO transmit data register to have space
        while (!uart_is_tx_empty(uart_ptr));
        uart_write(uart_ptr , *tx_packet++);
    }
}

/** brief Sends packet over USB */
static void usb_send_packet(void *pack_out, uint32_t length)
{
    while (!udi_cdc_is_tx_ready());
    udi_cdc_write_buf(pack_out, length);
}


/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief Communication initialization routine */
void comm_init()
{
    // TX buffer
    memset((void *)&packet_out , 0, MSG_LENGTH_MAX_SIZE);

    // UART PC variable
    memset((void *)&UART_PC_msg , 0, sizeof(UART_PC_msg));
    UART_PC_msg.uart_ptr = UART_PC;

    // USB PC variable
    memset((void *)&USB_PC_msg , 0, sizeof(USB_PC_msg));
    USB_PC_msg.uart_ptr = USB_PC;
}

/** @brief Background communication process */
void comm_background()
{
    comm_process_RX_uart_msg(&UART_PC_msg);
    comm_process_RX_usb_msg(&USB_PC_msg);
}

/** @brief Send message
  * @details This functions is called when a message have to be sent.
  *
  * @param[in] uart_ptr Pointer to destination UART
  * @param[in] msg_des Destination ID
  * @param[in] msg_id Message ID
  * @param[in] payload_len Payload size in uint8_t
  * @param[in] payload_ptr Pointer to payload
  */
void comm_send_msg(Uart *uart_ptr ,
                   uint8_t msg_des,
                   uint8_t msg_id ,
                   uint8_t payload_len ,
                   uint8_t * payload_ptr
                   )
{
    uint16_t checksum;

    // set flag of packet out busy
    packet_out_busy = 1;

    // Fill in output packet structure
    packet_out[msg_h_start] = MSG_START_BYTE;
    packet_out[msg_h_proj_id] = SETTINGS_PROJECT_ID;
    packet_out[msg_h_src_id] = SETTINGS_ARM_ID;
    packet_out[msg_h_des_id] = msg_des;
    packet_out[msg_h_msg_id] = msg_id;
    packet_out[msg_h_payload_len] = payload_len;

    // save payload
    copy_content(payload_ptr , &packet_out[msg_h_payload_1st_byte] , payload_len);

    // Append checksum
    checksum = calculate_checksum( packet_out , (uint16_t) (packet_out[msg_h_payload_len] + MSG_LENGTH_HEADER - 2));
    packet_out[MSG_LENGTH_HEADER - 2 + payload_len] = ( checksum      & 0xFF );
    packet_out[MSG_LENGTH_HEADER - 1 + payload_len] = ( (checksum>>8) & 0xFF );

    // Check if it's coming from UART or USB
    if (uart_ptr != NULL)
    {
        // Send packet via UART
        uart_send_packet( uart_ptr , packet_out , MSG_LENGTH_HEADER + payload_len );
    }
    else
    {
        // Send packet via USB
        usb_send_packet( (void*)&packet_out, (uint32_t) (MSG_LENGTH_HEADER + payload_len) );
    }
    // clear flag of packet out busy
    packet_out_busy = 0;
}

/** @brief Preprocess msg coming from the PC-uart */
void comm_preprocess_uart_PC_RX(void)
{
    comm_preprocess_RX_uart_msg(&UART_PC_msg);
}

/** @brief Preprocess msg coming from the PC-USB */
void comm_preprocess_usb_PC_RX(void)
{
    comm_preprocess_RX_usb_msg(&USB_PC_msg);
}
