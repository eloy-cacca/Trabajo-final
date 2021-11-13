// $Header
/// @file			DSP2833x_drivers_sci.c
/// @brief  		SCI drivers.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		26 October 2016
/// @details
/// @copyright

/// @addtogroup system
/// @{

/**************************** INCLUDES ***************************************/

#include "DSP2833x_drivers_sci.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/*************************** FUNCTION PROTOTYPES *****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

bool sci_check_errors(sci_reg_t *sci_ptr)
{

    if ((sci_ptr->SCIRXST.bit.RXERROR) ) {
        sci_ptr->SCICTL1.bit.SWRESET = 0; // Reset SCI and hold in reset
        sci_ptr->SCICTL1.bit.SWRESET = 1; // Re-enable SCI
        return 1;
    }
    else if (sci_ptr->SCIFFRX.bit.RXFFOVF ) {
    	sci_ptr->SCIFFRX.bit.RXFIFORESET = 0; // Reset SCI FIFO and hold in reset
    	sci_ptr->SCIFFRX.bit.RXFIFORESET = 1; // Re-enable SCI FIFO
    	return 1;
    }
    else
    	return 0;

}

bool sci_fifo_rx_data_available(sci_reg_t *sci_ptr) {

    return (sci_ptr->SCIFFRX.bit.RXFFST > 0);

}

bool sci_fifo_tx_is_full(sci_reg_t *sci_ptr) {

	return (sci_ptr->SCIFFTX.bit.TXFFST > 15);

}


// This function do not check if there is free space in FIFO
void sci_send_byte(sci_reg_t *sci_ptr, uint8_t data) {

	sci_ptr->SCITXBUF = data;

}

uint8_t sci_read_byte(sci_reg_t *sci_ptr) {

    return sci_ptr->SCIRXBUF.all;

}

void sci_send_packet(sci_reg_t *sci_ptr, const uint8_t *tx_packet, uint16_t tx_len) {

    while (tx_len--) {

        // wait for sci FIFO transmit data register to have space
        while (sci_ptr->SCIFFTX.bit.TXFFST > 15);

        sci_send_byte( sci_ptr , *tx_packet++);
    }

    //while (sci_ptr->SCIFFTX.bit.TXFFST != 0);

}

uint8_t sci_read_packet(sci_reg_t *sci_ptr, uint8_t *rx_packet, uint16_t rx_len) {
/*    uint8_t ret_value = STATUS_UART_SUCCESS;
    uint8_t dummy = 0;

    UART_Type *uart = (UART_Type *) uart_ptr;

    while (rx_len--) {
        while (!UART_BRD_S1_RDRF(uart));
        uint8_t aux = uart_read_byte(uart);
        *rx_packet = aux;
        rx_packet++;

        // Clear the Overrun flag since it will block receiving
        if (UART_BRD_S1_OR(uart)) {
            dummy = UART_RD_S1(uart);
            dummy = UART_RD_D(uart);
            UNUSED(dummy);
            ret_value = STATUS_UART_RXOVERRUN;
        }
    }

    return ret_value;*/
    return 0;

}


/// @}
