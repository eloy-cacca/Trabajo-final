// $Header
/// @file			system.c
/// @brief  		System apps.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 December 2016
/// @details
/// @copyright
///

/// @addtogroup modules
/// @{

/**************************** INCLUDES ***************************************/

#include "system.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

///@brief Configure LED pins
static void arm_leds_init(void) {
	
	// led 0		
	gpio_configure_pin(LED0_GPIO, LED0_FLAGS);
	gpio_set_pin_low(LED0_GPIO);                 // initial value

	// led 1
	gpio_configure_pin(LED1_GPIO, LED1_FLAGS);
	gpio_set_pin_low(LED1_GPIO);                 // initial value
	
}

///@brief Configure Testpoints pins
static void arm_testpoints_init(void) {
	
	// testpoint 0
	gpio_configure_pin(TP0_GPIO, TP0_FLAGS);
	gpio_set_pin_low(TP0_GPIO);                 // initial value

	// testpoint 1
	gpio_configure_pin(TP1_GPIO, TP1_FLAGS);
	gpio_set_pin_low(TP1_GPIO);                 // initial value
	
	// testpoint 2
	gpio_configure_pin(TP2_GPIO, TP2_FLAGS);
	gpio_set_pin_low(TP2_GPIO);                 // initial value
	
	// testpoint 3
	gpio_configure_pin(TP3_GPIO, TP3_FLAGS);
	gpio_set_pin_low(TP3_GPIO);                 // initial value

}

///@brief Configure Push Button pins
static void arm_push_button_init(void) {

	// Configure Push Button.
	// GPIO configuration
	gpio_configure_pin(GPIO_PUSH_BUTTON_1, GPIO_PUSH_BUTTON_1_FLAGS);
	// Adjust pio debounce filter parameters, uses 10 Hz filter.
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
	// Initialize pios interrupt handlers, see PIO definition in board.h.
	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID, PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR, coreio_push_button_1_isr);
	// enable push button 1 interrupt
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);

	// Configure Push Button.
	// GPIO configuration
	gpio_configure_pin(GPIO_PUSH_BUTTON_2, GPIO_PUSH_BUTTON_2_FLAGS);
	// Adjust pio debounce filter parameters, uses 10 Hz filter.
	pio_set_debounce_filter(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK, 10);
	// Initialize pios interrupt handlers, see PIO definition in board.h.
	pio_handler_set(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_ID, PIN_PUSHBUTTON_2_MASK, PIN_PUSHBUTTON_2_ATTR, coreio_push_button_2_isr);
	// enable push button 2 interrupt
	pio_enable_interrupt(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK);
	
}

///@brief UARTs initialization
static void arm_uart_init(void) {
	
	sam_uart_opt_t uart_settings;
	uart_settings.ul_mck = sysclk_get_peripheral_hz();
	uart_settings.ul_baudrate = SETTINGS_COMM_ARM_BAUDRATE;
	uart_settings.ul_mode = US_MR_CHRL_8_BIT |  // 8 bit lenght
							US_MR_PAR_NO |		// no parity
							US_MR_NBSTOP_1_BIT;	// 1 stop bit
	
	// Configure UART0 pins
	gpio_configure_group(PINS_UART0_PIO, PINS_UART0, PINS_UART0_FLAGS);
	// enable uart clk
	sysclk_enable_peripheral_clock(ID_UART0);
	// Configure UART
	uart_init( UART0 , &uart_settings);
	uart_enable( UART0 );
	uart_enable_interrupt( UART0 , UART_IER_RXRDY );

	// Configure UART1 pins
	gpio_configure_group(PINS_UART1_PIO, PINS_UART1, PINS_UART1_FLAGS);
	// enable uart clk
	sysclk_enable_peripheral_clock(ID_UART1);
	// Configure UART
	uart_init( UART1 , &uart_settings);
	uart_enable( UART1 );
	uart_enable_interrupt( UART1 , UART_IER_RXRDY );
	
}

///@brief HSMCI initialization - used with SD-Card
static void	arm_hsmci_init(void) {
	
	// GPIO initialization
	// Configure HSMCI pins
	gpio_configure_pin(PIN_HSMCI_MCCDA_GPIO, PIN_HSMCI_MCCDA_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCCK_GPIO, PIN_HSMCI_MCCK_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA0_GPIO, PIN_HSMCI_MCDA0_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA1_GPIO, PIN_HSMCI_MCDA1_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA2_GPIO, PIN_HSMCI_MCDA2_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA3_GPIO, PIN_HSMCI_MCDA3_FLAGS);
	// Configure SD/MMC card detect pin
	gpio_configure_pin(SD_MMC_0_CD_GPIO, SD_MMC_0_CD_FLAGS);

	// peripheral initialization
	sd_mmc_init();
}

/// @brief Real Time Clock Initialization
static void arm_rtc_init(void) {
	
	pmc_switch_sclk_to_32kxtal(0);		// Select 32.768 KHz crystal as Slow CLock source. No bypass?????	
	while (pmc_osc_is_ready_32kxtal() != 1) {}
	rtc_set_hour_mode(RTC, 0); 			// Default RTC configuration, 24-hour mode
	//rtc_set_time(RTC,12,50,0);		// Hour: 12:50:0
	//rtc_set_date(RTC,2014,7,16,5);	// Date: 2014/07/16 and Thursday=5 (user-defined)
	rtc_set_calibration(RTC, 1 , 124 , 1);	// 0   -> Positive correction //TODO: creo que debe ser negativo pero me duplica el error.
											// 124 -> correction
											// 1   -> HIGH PPM Correction	

}

/// @brief Supply monitor Initialization
static void	arm_supply_monitor_init(void) {
	
	// Set Supply monitor to generate an IRQ when a supply monitor detection occurs.
/*	SUPC->SUPC_SMMR =	SUPC_SMMR_SMRSTEN_NOT_ENABLE |  // The core reset signal vddcore_nreset is not affected when a supply monitor detection occurs.
						SUPC_SMMR_SMIEN_ENABLE |        // Supply Monitor Interrupt Enable.
						SUPC_SMMR_SMSMPL_2048SLCK |     // Supply Monitor enabled one SLCK period every 2048 SLCK periods.
						//SUPC_SMMR_SMTH(0x09u);        // 0x09u -> Threshold of 2.68v (approximately between 2.61 and 2.75).
						SUPC_SMMR_SMTH(0x0Au);          // 0x0Au -> Threshold of 2.80v (approximately between 2.73 and 2.87).
						//SUPC_SMMR_SMTH(0x0Cu);        // 0x0Cu -> Threshold of 3.04v (approximately between 2.96 and 3.12).
						//SUPC_SMMR_SMTH(0x0Du);        // 0x0Du -> Threshold of 3.16v (approximately between 3.08 and 3.24).
*/						
	supc_disable_monitor_reset( SUPC );
	supc_enable_monitor_interrupt( SUPC );
	supc_set_monitor_sampling_period( SUPC , SUPC_SMMR_SMSMPL_2048SLCK );
	supc_set_monitor_threshold( SUPC , 0x0Au  );
	
}

static void arm_toggle_leds_3_times(void) {

	uint8_t repeat;

	// turn on and off leds
	for ( repeat=0 ; repeat<3 ; repeat++ ) {
		// leds on
		led_green_on();
		led_red_on();
		delay_ms(250);
		// leds off
		led_green_off();
		led_red_off();
		delay_ms(250);
	}

}

/**************************** GLOBAL FUNCTIONS *******************************/

///@brief System initialization.
void system_init() {

	// general initialization
	WDT->WDT_MR = WDT_MR_WDDIS; // Disable the watchdog
	sysclk_init();
	sysclk_enable_peripheral_clock(ID_PIOA);
//	sysclk_enable_peripheral_clock(ID_PIOB);
	gpio_configure_group( PIOA , 0xFFFFFFFF , (PIO_INPUT | PIO_DEFAULT));
	gpio_configure_group( PIOB , 0xFFFFFFFF , (PIO_INPUT | PIO_DEFAULT));
	pio_pull_down( PIOA , 0xFFFFFFFF , 0 );
	pio_pull_down( PIOB , 0xFFFFFFFF , 0 );
	pio_pull_up( PIOA , 0xFFFFFFFF , 0 );
	pio_pull_up( PIOB , 0xFFFFFFFF , 0 );
		
	// specific initialization
	arm_rtc_init();	
	arm_leds_init();
	arm_testpoints_init();
	arm_uart_init();
	arm_push_button_init();
	arm_hsmci_init();
	arm_supply_monitor_init();
	 
 }

///@brief Show that system is ready. 
void system_ready(void) {

	arm_toggle_leds_3_times();

}

///@brief Enable interrupts.
void system_enable_interrupt(void) {

	// Enable Supply monitor
	NVIC_DisableIRQ((IRQn_Type) SUPC_IRQn);
	NVIC_ClearPendingIRQ((IRQn_Type) SUPC_IRQn);
	NVIC_SetPriority((IRQn_Type) SUPC_IRQn,0);
	NVIC_EnableIRQ((IRQn_Type) SUPC_IRQn);

	// Enable PIO controller IRQs.
	NVIC_EnableIRQ((IRQn_Type)PIN_PUSHBUTTON_1_ID);
	NVIC_ClearPendingIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
	NVIC_SetPriority((IRQn_Type) PIN_PUSHBUTTON_1_ID,3);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
	
	// Enable PIO controller IRQs.
	NVIC_EnableIRQ((IRQn_Type)PIN_PUSHBUTTON_2_ID);	
	NVIC_ClearPendingIRQ((IRQn_Type) PIN_PUSHBUTTON_2_ID);
	NVIC_SetPriority((IRQn_Type) PIN_PUSHBUTTON_2_ID,4);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_2_ID);
	
	// Enable uart 0
	NVIC_DisableIRQ((IRQn_Type) UART0_IRQn);
	NVIC_ClearPendingIRQ((IRQn_Type) UART0_IRQn);
	NVIC_SetPriority((IRQn_Type) UART0_IRQn,1);
	NVIC_EnableIRQ((IRQn_Type) UART0_IRQn);
	
	// Enable uart 1
	NVIC_DisableIRQ((IRQn_Type) UART1_IRQn);
	NVIC_ClearPendingIRQ((IRQn_Type) UART1_IRQn);
	NVIC_SetPriority((IRQn_Type) UART1_IRQn,2);
	NVIC_EnableIRQ((IRQn_Type) UART1_IRQn);
	
}

///@brief Disable interrupts.
void system_disable_interrupt(void) {
	
	NVIC_DisableIRQ((IRQn_Type) SUPC_IRQn);
	NVIC_DisableIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
	NVIC_DisableIRQ((IRQn_Type) PIN_PUSHBUTTON_2_ID);	
	NVIC_DisableIRQ((IRQn_Type) UART0_IRQn);
	NVIC_DisableIRQ((IRQn_Type) UART1_IRQn);
			
}

///@brief Reset ARM.
void system_reset_ARM(void) {
	
	RSTC->RSTC_CR = RSTC_CR_KEY_PASSWD | RSTC_CR_PROCRST | RSTC_CR_PERRST;

}


/// @}

