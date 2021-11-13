// $Header
/// @file		    main.c
/// @brief          main function.
/// @details
/// @author         Ignacio Carugati
/// @version        1.0
/// @date           22 December 2016
/// @details
/// @copyright
///

/// @addtogroup main
/// @{

/*************************** INCLUDES ****************************************/

#include "settings.h"

#include "apps/system.h"
#include "apps/bootloader.h"
#include "apps/sdcard.h"
#include "apps/unit_config.h"
#include "apps/supply_monitor.h"

#include "comm/comm.h"

/*************************** MACROS ******************************************/

/*************************** ENUMS, STRUCTS **********************************/

/*************************** FUNCTION PROTOTYPES *****************************/

/*************************** LOCAL VARIABLES *********************************/

/*************************** GLOBAL VARIABLES ********************************/

/*************************** LOCAL FUNCTIONS *********************************/

/*************************** GLOBAL FUNCTIONS ********************************/

///@brief 	Main function.
///@details Application entry point.
int main(void)
{
	
	// Initialization
	system_init();
	bootloader_init();
	unit_config_and_calibration_init();
	comm_init();
	
	// system ready
	system_ready();
	SysTick_Config( sysclk_get_cpu_hz()/SETTINGS_ARM_TIMER_FREQ_HZ ); // set clock timer and enable interrupt
	system_enable_interrupt();
	
	while(1) {
	
		sdcard_background();
		delay_ms(500);
		gpio_set_pin_high(LED0_GPIO);
		delay_ms(500);
		gpio_set_pin_low(LED0_GPIO);
		comm_background();
		
    }
}
						
///@brief Push Button Interrupt Service Routine.
///@details Enable/Disable the detection of SD-Card. Set this flag force
///         detection in background and clear this flag force a idle state
void coreio_push_button_1_isr(uint32_t id, uint32_t mask) {
	
	if ((PIN_PUSHBUTTON_1_ID == id) && (PIN_PUSHBUTTON_1_MASK == mask)) {
		if (sdcard_flags_status & SDCARD_FLAGS_STATUS_ENABLE_SDCARD )
			sdcard_flags_status &= ~SDCARD_FLAGS_STATUS_ENABLE_SDCARD;
		else
			sdcard_flags_status |= SDCARD_FLAGS_STATUS_ENABLE_SDCARD;
	}
}

///@brief Push Button Interrupt Service Routine.
///@details Enable/Disable the detection of SD-Card. Set this flag force
///         detection in background and clear this flag force a idle state
///@note Other action can be assigned in a following revision
void coreio_push_button_2_isr(uint32_t id, uint32_t mask) {
	
	if ((PIN_PUSHBUTTON_2_ID == id) && (PIN_PUSHBUTTON_2_MASK == mask)) {
				
		if (sdcard_flags_status & SDCARD_FLAGS_STATUS_ENABLE_SDCARD )
			sdcard_flags_status &= ~SDCARD_FLAGS_STATUS_ENABLE_SDCARD;
		else
			sdcard_flags_status |= SDCARD_FLAGS_STATUS_ENABLE_SDCARD;
	}
}

///@brief Handler for System Tick interrupt.
///@details Process System Tick Event. Update counters.
void SysTick_Handler(void) {
	
	comm_counters_update();
	
}

///@brief Handler for UART 0.
void UART0_Handler(void) {
	comm_preprocess_uart_DSP_RX();
}

///@brief Handler for UART 1.
void UART1_Handler(void) {
	comm_preprocess_uart_RPI_RX();
}


///@brief Interrupt Handler for Supply monitor (SUPC)
///@details It is executed when a supply monitor detection occurs.
///         Go to backup model.
void SUPC_Handler(void)
{

	uint32_t supc_sr_value = 0;
	
	supc_sr_value = SUPC->SUPC_SR;
	
	if (supc_sr_value & SUPC_SR_SMS)
	{
 		
 		// Disable clocks and correctly configure I/Os to reduce power consumption.
 		backup_mode_prepare();
		
 		// Enter in back up mode till power is over threshold again
 		enter_backup_mode();
	}
}


/// @}
