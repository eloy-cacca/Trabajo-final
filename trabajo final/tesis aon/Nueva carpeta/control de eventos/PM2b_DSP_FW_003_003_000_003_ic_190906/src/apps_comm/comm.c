// $Header
/// @file			comm.c
/// @brief  		Communication file.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		27 October 2016
/// @details
/// @copyright
///

/// @addtogroup comm
/// @{

/**************************** INCLUDES ***************************************/

// Specific includes
#include "comm.h"

// Local includes
#include "app_layer.h"

#include "../utils/byte_string_operations.h"

#include "../apps/unit_config.h"
#include "../apps/control.h"
#include "../apps/rtc.h"

#include "../apis/fw_api.h"
#include "../apis/rtc_api.h"
#include "../apis/unit_config_api.h"
#include "../apis/control_api.h"


/**************************** MACROS *****************************************/

#define MSG_TIMEOUT         ( 0x000007FF )

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

// Sci variables
msg_t SCIA_msg;
//msg_t SCIB_msg;
//msg_t SCIC_msg;

// output
uint8_t  packet_out[SETTINGS_MSG_LENGHT_MAX_SIZE];
uint16_t packet_out_busy = 0;

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/// @brief Generic SCI pre-process incoming message
/// @details This functions is called by Rx interrupt when a byte is received.
///          Byte is added to buffer and counter of received data is updated.
/// @param[in] msg_ptr Pointer to SCI structure
void comm_preprocess_msg( msg_t * msg_ptr ) {

	// Get bytes from FIFO
	while ( sci_fifo_rx_data_available(msg_ptr->sci_ptr) ) {

		// save new byte
		msg_ptr->packet_in[msg_ptr->rx_counter] = sci_read_byte( msg_ptr->sci_ptr );

		// increment the rx counter and
		msg_ptr->rx_counter++;

	}

	// reset timeout counter
	msg_ptr->timeout_counter = 0;

	// clear SCI FIFO interrupt flag
	msg_ptr->sci_ptr->SCIFFRX.bit.RXFFINTCLR = 1;

}

/// @brief Generic SCI process incoming message
/// @details This functions is called in background and check if a message is complete.
/// @param[in] msg_ptr Pointer to SCI structure
void comm_process_msg_RX( msg_t * msg_ptr ) {

	// If error occurs, start over.
    if ( sci_check_errors(msg_ptr->sci_ptr) ) {

    	memset( msg_ptr->packet_in , 0 , SETTINGS_MSG_LENGHT_MAX_SIZE );  // Reset buffer
        msg_ptr->rx_counter = 0;                          // Reset rx index
        msg_ptr->timeout_counter = 0;                     // Reset timeout counter

    }

    // Increment timeout counter
    msg_ptr->timeout_counter++;

    // Reset counters if timeout occurs
    if (msg_ptr->timeout_counter == MSG_TIMEOUT) {
    	msg_ptr->timeout_counter = 0;
    	msg_ptr->rx_counter = 0;
    }

   	// Check proper start byte
   	if (msg_ptr->rx_counter > 0) {
   		if (msg_ptr->packet_in[msg_h_start] != SETTINGS_MSG_START_BYTE)
   			msg_ptr->rx_counter = 0; // reset buffer
   	}

   	// Check proper project id
   	if (msg_ptr->rx_counter > 1) {
       	if (!(msg_ptr->packet_in[msg_h_proj_id] == SETTINGS_PROJECT_ID) )
       		msg_ptr->rx_counter = 0; // reset buffer
   	}

   	// Check proper source id
   	if (msg_ptr->rx_counter > 2) {
       	if (!( (msg_ptr->packet_in[msg_h_src_id] == SETTINGS_OTHERS_ID) ||
       		   (msg_ptr->packet_in[msg_h_src_id] == SETTINGS_ARM_ID) ||
               (msg_ptr->packet_in[msg_h_src_id] == SETTINGS_RPI_ID) ))
       		msg_ptr->rx_counter = 0; // reset buffer
   	}

   	// Check proper destination id
   	if (msg_ptr->rx_counter > 3) {
       	if (!( (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_OTHERS_ID) ||
       		   (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_ARM_ID) ||
       		   (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_RPI_ID) ||
			   (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_DSP_ID)))
       		msg_ptr->rx_counter = 0; // reset buffer
   	}

   	// if the packet is complete, check CRC and packet out is not busy, decode message and reset buffer
   	if (  (msg_ptr->rx_counter >= SETTINGS_MSG_LENGHT_HEADER) &&
   		  ((msg_ptr->packet_in[msg_h_payload_len] + SETTINGS_MSG_LENGHT_HEADER) == msg_ptr->rx_counter) &&
          (!packet_out_busy ) &&
   		  (!calculate_checksum( msg_ptr->packet_in , (uint16_t) msg_ptr->rx_counter)) ) {
       		decode_incoming_comms_packet( msg_ptr );
       	msg_ptr->rx_counter = 0; // reset buffer
   	}

}

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Communication initialization routine.
void comm_init(void) {

	// TX buffer
	memset( (void *)&packet_out , 0 , SETTINGS_MSG_LENGHT_MAX_SIZE );

	// SCI-A variable initialization
	memset( (void *)&SCIA_msg , 0 , sizeof(SCIA_msg) );
	SCIA_msg.sci_ptr = &SCIA;

	// SCI-B variable initialization
//	memset( (void *)&SCIB_msg , 0 , sizeof(SCIB_msg) );
//	SCIB_msg.sci_ptr = &SCIB;

	// SCI-C variable initialization
//	memset( (void *)&SCIC_msg , 0 , sizeof(SCIC_msg) );
//	SCIC_msg.sci_ptr = &SCIC;

}


/// @brief Communication background routine.
void comm_background(void) {

    // Run SCI-A RX routine
	comm_process_msg_RX( &SCIA_msg );

    // Run SCI-B RX routine
//	comm_process_msg_RX( &SCIB_msg );

    // Run SCI-C RX routine
//	comm_process_msg_RX( &SCIC_msg );

#ifdef DELETE
	// Process pending communication depending on dsp flags
	{

		if ( (!packet_out_busy) && (rtc_alarm2000msecond_flag()) ) {

			// end message
			if      (comm_flags.init & DSP_FLAG_SEND_START_CMD_MASK) {
				rtc_alarm2000msecond_start();
				API_FW_unit_start_send();
			}
			else if (comm_flags.init & DSP_FLAG_RTC_REQUEST_MASK) {
				rtc_alarm2000msecond_start();
				API_RTC_date_update_req();
			}
			else if (comm_flags.init & DSP_FLAG_UC_REQUEST_MASK) {
				rtc_alarm2000msecond_start();
				API_UC_unit_config_update_req(  );
			}
			else if (comm_flags.trend_150cycles > 0) {
				rtc_alarm2000msecond_start();
				API_CTRL_trend_send( ctrl_ptr_to_trend_150cycles() , comm_flags.trend_150cycles , API_CTRL_TREND_150CYCLES_START_SEND);
			}
			else if (comm_flags.trend_5minutes > 0) {
				rtc_alarm2000msecond_start();
				API_CTRL_trend_send( ctrl_ptr_to_trend_5m() , comm_flags.trend_5minutes , API_CTRL_TREND_5MINUTES_START_SEND);
			}
			else if (comm_flags.telemetry > 0) {
				rtc_alarm2000msecond_start();
				API_CTRL_trend_send( ctrl_ptr_to_trend_150cycles() , comm_flags.trend_150cycles , API_CTRL_TREND_TELEMETRY_START_SEND); // DEFAULT: use 150 cycles trend
			}
			else if (comm_flags.event > 0) {
				rtc_alarm2000msecond_start();
				//TODO
			}


		}

	}
#endif

}


/// @brief Send message
/// @details This functions is called when a message have to be sent.
///
/// @param[in] uart_ptr Pointer to destination SCI
/// @param[in] msg_des Destination ID
/// @param[in] msg_id Message ID
/// @param[in] payload_len Payload size in uint8_t
/// @param[in] payload_ptr Pointer to payload
/// @param[in] data_type Payload must be interpreted as 16bits or 8 bits word
void comm_send_message( sci_reg_t *uart_ptr ,
		                uint8_t msg_des,
		                uint8_t msg_id ,
		                uint8_t payload_len ,
		                uint8_t * payload_ptr
		               ) {

	 uint16_t checksum;

	 // set flag of packet out busy
	 packet_out_busy = 1;

	 // Fill in output packet structure
	 packet_out[msg_h_start] = SETTINGS_MSG_START_BYTE;
	 packet_out[msg_h_proj_id] = SETTINGS_PROJECT_ID;
	 packet_out[msg_h_src_id] = SETTINGS_DSP_ID;
	 packet_out[msg_h_des_id] = msg_des;
	 packet_out[msg_h_msg_id] = msg_id;
	 packet_out[msg_h_payload_len] = payload_len;

	 // save payload
	 copy_content_form_16bits_to_8bits( payload_ptr , &packet_out[msg_h_payload_1st_byte] , payload_len );

     // Append checksum
     checksum = calculate_checksum( packet_out , (uint16_t) (packet_out[msg_h_payload_len] + SETTINGS_MSG_LENGHT_HEADER - 2));
	 packet_out[SETTINGS_MSG_LENGHT_HEADER - 2 + payload_len] = ( checksum      & 0xFF );
	 packet_out[SETTINGS_MSG_LENGHT_HEADER - 1 + payload_len] = ( (checksum>>8) & 0xFF );

     // Send packet via USB UART
	 sci_send_packet( uart_ptr , packet_out , SETTINGS_MSG_LENGHT_HEADER + payload_len );
	 // clear flag of packet out busy
	 packet_out_busy = 0;

}

#if 0
/// @brief Send requeste when a received message must be bypassed to other device (VFP to FPGA or FPGA to VFP)
/// @details This functions is called when a well-formed message is received and it must be transmitted to other device.
/// @param[in] msg_des Pointer to received message
/// @param[in] uart Pointer to destination UART
void comm_send_request( msg_in_t * msg_ptr , void * uart ) {

	 // save packet
	 copy_content( msg_ptr->packet_in , &packet_out[0] , msg_ptr->rx_counter );

     // Send packet via USB UART
	 {
		 uart_transfer_t xfer;
	     xfer.data = packet_out;
	     xfer.dataSize = (uint8_t) (msg_ptr->rx_counter);
	     if ( uart == VFP_UART)
	      	UART_SendEDMA( (UART_Type *)uart , &VFP_uartEdmaHandle, &xfer);
	     else
	       	UART_SendEDMA( (UART_Type *)uart , &FPGA_uartEdmaHandle, &xfer);
	 }

}
#endif

/// @brief SCI A RX - Read data
void comm_sci_a_rx(void) {

	comm_preprocess_msg(&SCIA_msg);

}

/// @brief SCI B RX - Read data
/*void comm_sci_b_rx(void) {

	comm_preprocess_msg(&SCIB_msg);

}*/

/// @brief SCI C RX - Read data
/*void comm_sci_c_rx(void) {

	comm_preprocess_msg(&SCIC_msg);

}*/




/// @}

