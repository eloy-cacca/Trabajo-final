// $Header
/// @file			DSP2833x_drivers_sci.h
/// @brief  		SCI drivers.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		26 October 2016
/// @details
/// @copyright
///

#ifndef __DSP28335_DRIVERS_SCI_H__
#define __DSP28335_DRIVERS_SCI_H__

/**************************** INCLUDES ***************************************/

#include "DSP2833x_library.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

typedef volatile struct SCI_REGS sci_reg_t;

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

bool sci_check_errors(sci_reg_t *sci_ptr);

bool sci_fifo_rx_data_available(sci_reg_t *sci_ptr);
bool sci_fifo_tx_is_full(sci_reg_t *sci_ptr);

void sci_send_byte(sci_reg_t *sci_ptr, uint8_t data);
uint8_t sci_read_byte(sci_reg_t *sci_ptr);
void sci_send_packet(sci_reg_t *sci_ptr, const uint8_t *tx_packet, uint16_t tx_len);
uint8_t sci_read_packet(sci_reg_t *sci_ptr, uint8_t *rx_packet, uint16_t rx_len);

/// @endcond

#endif /* __DSP28335_DRIVERS_SCI_H__ */
