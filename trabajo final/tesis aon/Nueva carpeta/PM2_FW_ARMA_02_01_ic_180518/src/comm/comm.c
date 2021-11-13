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
#include "../apps/sdcard.h"
#include "../apps/unit_config.h"
#include "../apis/dsp_api.h"
#include "../apps/system.h"
/*

#include "../mod/control.h"

#include "../apis/fw_api.h"
#include "../apis/rtc_api.h"
#include "../apis/unit_config_api.h"
#include "../apis/control_api.h"

#include "../apps/rtc.h"*/

/**************************** MACROS *****************************************/

#define MSG_START_BYTE      ((uint8_t)(0x3C))
#define MSG_TIMEOUT         ( 0x000007FF )

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

void comm_preprocess_RX_msg( msg_t * msg_ptr );
void comm_process_RX_msg( msg_t * msg_ptr );
void uart_send_packet(Uart *uart_ptr, uint8_t *tx_packet, uint16_t tx_len);

/**************************** LOCAL VARIABLES ********************************/

// Sci variables
msg_t UART_RPI_msg;
msg_t UART_DSP_msg;

// output
uint8_t packet_out[MSG_LENGHT_MAX_SIZE];
bool    packet_out_busy = 0;

// Counter used for TX pending communication
uint16_t comm_TX_counter;

// Counter used for request DSP comm flags
uint16_t comm_DSP_counter;

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/


/// @brief Generic UART pre-process incoming message
/// @details This functions is called by Rx interrupt when a byte is received.
///          Byte is added to buffer and counter of received data is updated.
/// @param[in] msg_ptr Pointer to msg_t
void comm_preprocess_RX_msg( msg_t * msg_ptr ) {

	// Check if a byte was received
	if ( uart_is_rx_ready(msg_ptr->uart_ptr) ) {

		// save new byte
		uart_read(msg_ptr->uart_ptr, &msg_ptr->packet_in[msg_ptr->rx_counter] );

		// increment the rx counter and
		msg_ptr->rx_counter++;

		// reset timeout counter
		msg_ptr->timeout_counter = 0;

	}

}


/// @brief Generic UART process incoming message
/// @details This functions is called in background and check if a message is complete.
/// @param[in] msg_ptr Pointer to UART structure
void comm_process_RX_msg( msg_t * msg_ptr ) {

	// If error occurs, start over.
    if ( uart_get_status(msg_ptr->uart_ptr) == (UART_SR_OVRE | UART_SR_FRAME | UART_SR_PARE) ) {

    	memset( msg_ptr->packet_in , 0 , MSG_LENGHT_MAX_SIZE ); // Reset buffer
        msg_ptr->rx_counter = 0;                                // Reset rx index
        msg_ptr->timeout_counter = 0;                           // Reset timeout counter
        uart_reset_status(msg_ptr->uart_ptr);					// Clear error since it will block receiving
																// TODO: this is e problem for other uC.

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
   		if (msg_ptr->packet_in[msg_h_start] != MSG_START_BYTE)
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
       		   (msg_ptr->packet_in[msg_h_src_id] == SETTINGS_DSP_ID) ||
               (msg_ptr->packet_in[msg_h_src_id] == SETTINGS_RPI_ID) ))
       		msg_ptr->rx_counter = 0; // reset buffer
   	}

   	// Check proper destination id
   	if (msg_ptr->rx_counter > 3) {
		    //	{
			//		#include "../apps/system.h"
			//		led_green_on();
		   	//	}		   
       	if (!( (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_ARM_ID) ||
       		   (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_RPI_ID) ||
			   (msg_ptr->packet_in[msg_h_des_id] == SETTINGS_DSP_ID)))
       		msg_ptr->rx_counter = 0; // reset buffer
   	}

   	// if the packet is complete, check CRC and packet out is not busy, decode message and reset buffer
   	if (  (msg_ptr->rx_counter >= MSG_LENGHT_HEADER) &&
   		  ((msg_ptr->packet_in[msg_h_payload_len] + MSG_LENGHT_HEADER) == msg_ptr->rx_counter) &&
          (!packet_out_busy ) &&
   		  (!calculate_checksum( msg_ptr->packet_in , (uint16_t) msg_ptr->rx_counter)) ) {
       		decode_incoming_comms_packet( msg_ptr );
       	msg_ptr->rx_counter = 0; // reset buffer
   	}

}

void uart_send_packet(Uart *uart_ptr, uint8_t *tx_packet, uint16_t tx_len) {
	
	while (tx_len--) {

		// wait for sci FIFO transmit data register to have space
        while ( !uart_is_tx_empty(uart_ptr) );

		uart_write( uart_ptr , *tx_packet++ );
	
	}
	
}

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Communication initialization routine.
void comm_init(void) {

	// TX buffer
	memset( (void *)&packet_out , 0 , MSG_LENGHT_MAX_SIZE );

	// UART DB15 variable initialization
	memset( (void *)&UART_RPI_msg , 0 , sizeof(UART_RPI_msg) );
	UART_RPI_msg.uart_ptr = UART_RPI;

	// UART DSP variable initialization
	memset( (void *)&UART_DSP_msg , 0 , sizeof(UART_DSP_msg) );
	UART_DSP_msg.uart_ptr = UART_DSP;
	
	// init counters
	comm_TX_counter = 0;
	comm_DSP_counter = 0;

}


/// @brief Communication background routine.
/// @details It run several function related to communication:
///          1) Process incoming msg from DSP
///          2) Run the following by priority
///             2.1) Pending communication with DSP
///             2.2) Process incoming msg from RPI
void comm_background(void) {

	// TODO: pass this function to DMA interruption routine
	//comm_preprocess_RX_msg( &UART_DSP_msg );
	//comm_preprocess_RX_msg( &UART_RPI_msg );
	
    // Process message from UART_DSP (run it always)
	comm_process_RX_msg( &UART_DSP_msg );

	// Run other communication function depending on priority, i.e., ones
	// a function is complete, do the intermediately next.
	// Process message from UART_RPI is the last since it has the lower
	// priority
	{

		// if output buffer is empty and a save time had happened from last transmition do the following
/*		if ( (!packet_out_busy) && (comm_TX_counter==0) ) {
		
			// Request DSP FW version
			if (comm_flags & SETTINGS_COMM_FLAG_FW_REQUEST_MASK) {
				gpio_set_pin_high(TP0_GPIO);
				API_FW_DSP_version_req();
				gpio_set_pin_low(TP0_GPIO);
			}
			// Request DSP flags of pending communication
				else if (comm_flags & SETTINGS_COMM_FLAG_REQUEST_MASK) {
				gpio_set_pin_high(TP0_GPIO);
				API_FW_DSP_send_comm_flag_req();
				gpio_set_pin_low(TP0_GPIO);
			}
			// Send RTC
			else if (comm_flags & SETTINGS_COMM_FLAG_RTC_REQUEST_MASK) {
				gpio_set_pin_high(TP1_GPIO);
				API_RTC_DSP_update_req();
				gpio_set_pin_low(TP1_GPIO);
			}
			// Send unit configuration
			else if ((comm_flags & SETTINGS_COMM_FLAG_UNIT_CONFIG_REQUEST_MASK) && (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY) ) {
				gpio_set_pin_high(TP1_GPIO);
				API_UNIT_CONFIG_DSP_update_req();
				gpio_set_pin_low(TP1_GPIO);
			}
			// Send unit calibration
			else if ((comm_flags & SETTINGS_COMM_FLAG_UNIT_CALIBRATION_REQUEST_MASK) && (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY) ) {
				gpio_set_pin_high(TP1_GPIO);
				API_UNIT_CALIBRATION_DSP_update_req();
				gpio_set_pin_low(TP1_GPIO);
			}
			// Request trend 150 cycles
			else if ( (comm_flags & SETTINGS_COMM_FLAG_TREND_150CYCLE_MASK) && (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY) ) {
				gpio_set_pin_high(TP2_GPIO);
				API_CTRL_DSP_trend_150cycles_req();
				gpio_set_pin_low(TP2_GPIO);
			}
			// Request trend 5 minutes
			else if ( (comm_flags & SETTINGS_COMM_FLAG_TREND_5MINUTES_MASK) && (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY) ) {
				gpio_set_pin_high(TP2_GPIO);
				API_CTRL_DSP_trend_5minutes_req();
				gpio_set_pin_low(TP2_GPIO);				
			}
			// Request telemetry
//			else if (comm_flags & COMM_DSP_FLAG_TELEMETRY_MASK) {
//				//API_FW_unit_start_send();
//				comm_TX_counter_set();
//			}						
			// Request event/snapshot
			else if ( (comm_flags & SETTINGS_COMM_FLAG_EVENT_DET_MASK) && (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY) ) {
				gpio_set_pin_high(TP3_GPIO);
				API_CTRL_DSP_event_det_req();
				gpio_set_pin_low(TP3_GPIO);
			}
			// Request event/snapshot
			else if ( (comm_flags & SETTINGS_COMM_FLAG_EVENT_BUFFER_MASK) && (sdcard_flags_status & SDCARD_FLAGS_STATUS_CARD_READY) ) {
				gpio_set_pin_high(TP3_GPIO);
				API_CTRL_DSP_event_buffer_req();
				gpio_set_pin_low(TP3_GPIO);
			}
			// if nothing to do, process msg from RPI
			else*/
				comm_process_RX_msg( &UART_RPI_msg );

	//	}
		
	}
			
}


/// @brief Send message
/// @details This functions is called when a message have to be sent.
///
/// @param[in] uart_ptr Pointer to destination UART
/// @param[in] msg_des Destination ID
/// @param[in] msg_id Message ID
/// @param[in] payload_len Payload size in uint8_t
/// @param[in] payload_ptr Pointer to payload
void comm_send_msg( Uart *uart_ptr ,
		            uint8_t msg_des,
		            uint8_t msg_id ,
		            uint8_t payload_len ,
		            uint8_t * payload_ptr
		            ) {

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
	 copy_content( payload_ptr , &packet_out[msg_h_payload_1st_byte] , payload_len );

     // Append checksum
     checksum = calculate_checksum( packet_out , (uint16_t) (packet_out[msg_h_payload_len] + MSG_LENGHT_HEADER - 2));
	 packet_out[MSG_LENGHT_HEADER - 2 + payload_len] = ( checksum      & 0xFF );
	 packet_out[MSG_LENGHT_HEADER - 1 + payload_len] = ( (checksum>>8) & 0xFF );

     // Send packet via UART
	 uart_send_packet( uart_ptr , packet_out , MSG_LENGHT_HEADER + payload_len );
	
	 // clear flag of packet out busy
	 packet_out_busy = 0;

}


/// @brief Send request when a received message must be bypassed to other device (VFP to FPGA or FPGA to VFP)
/// @details This functions is called when a well-formed message is received and it must be transmitted to other device.
///
/// @param[in] uart_ptr Pointer to destination UART
/// @param[in] msp_ptr Pointer to incoming message
void comm_bypass_msg( Uart *uart_ptr , msg_t * msg_ptr ) {

	uint8_t payload_len;

	 // set flag of packet out busy
	 packet_out_busy = 1;

	 // save packet
	 copy_content( msg_ptr->packet_in , &packet_out[0] , msg_ptr->rx_counter );
	 payload_len = msg_ptr->rx_counter;
     
	 // Send packet via UART
     uart_send_packet( uart_ptr , packet_out , payload_len );
     
     // clear flag of packet out busy
     packet_out_busy = 0;

}

/// @brief Update counters
void comm_counters_update(void) {

	// Decrease the pending communication counter to repeat a TX if response is not received.
	if (comm_TX_counter>0)
		comm_TX_counter--;
	else
		comm_TX_counter = 0;
		
	// Increase DSP communication flag counter
	comm_DSP_counter++;
		
	// if DSP communication flag counter reach 1 second, update ARM communication flags
	// to force the request in background
	if (comm_DSP_counter<5000) {
		led_red_on();
	}
	else if (comm_DSP_counter<10000) {
		led_red_off();
	}
	else {
		comm_DSP_counter = 0;
		comm_flags |= SETTINGS_COMM_FLAG_REQUEST_MASK;
	}
	
}

/// @brief Clear pending communication counter
void comm_TX_counter_clear(void) {

	comm_TX_counter = 0;

}

/// @brief Set pending communication counter to 0.5 second
void comm_TX_counter_set(void) {

	comm_TX_counter = 5000;

}


void comm_preprocess_uart_DSP_RX(void) {
	comm_preprocess_RX_msg( &UART_DSP_msg );
}

void comm_preprocess_uart_RPI_RX(void) {
	comm_preprocess_RX_msg( &UART_RPI_msg );
}

/// @}
