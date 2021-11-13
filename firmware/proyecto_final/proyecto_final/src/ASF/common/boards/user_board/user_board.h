/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

/*----------------------------------------------------------------------------*/
/**
 *  \page sam4s_ek_opfreq "SAM4S-EK - Operating frequencies"
 *  This page lists several definition related to the board operating frequency
 *
 *  \section Definitions
 *  - \ref BOARD_FREQ_*
 *  - \ref BOARD_MCK
 */

/** Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL        (32768U)	//32.768 KHz
#define BOARD_FREQ_SLCK_BYPASS      (32768U)	//32.768 KHz
#define BOARD_FREQ_MAINCK_XTAL      (12000000U) //12 MHz
#define BOARD_FREQ_MAINCK_BYPASS    (12000000U) //12 MHz 

/** Master clock frequency */
#define BOARD_MCK                   CHIP_FREQ_CPU_MAX //120 MHz

/** board main clock xtal startup time */
#define BOARD_OSC_STARTUP_US   15625

/*----------------------------------------------------------------------------*/
/**
 * \page sam4s_ek_board_info "SAM4S-EK - Board informations"
 * This page lists several definition related to the board description.
 *
 * \section Definitions
 * - \ref BOARD_NAME
 */

/** Name of the board */
#define BOARD_NAME "USER-BOARD"
/** Board definition */
#define sam4sek
/** Family definition (already defined) */
#define sam4s
/** Core definition */
#define cortexm4

/************************************************************************/
/*			UART0 pins (UTXD0 and URXD0) definitions, PA9,10.           */                 
/************************************************************************/
#define PINS_UART0					(PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)  // define pins RX,TX
#define PINS_UART0_FLAGS			(PIO_PERIPH_A | PIO_DEFAULT)		// controlado por periferico A, sin atributos
#define PINS_UART0_MASK				PIO_PA9A_URXD0|PIO_PA10A_UTXD0		// mascara
#define PINS_UART0_PIO				PIOA								//define pio
#define PINS_UART0_ID				ID_PIOA								//ID
#define PINS_UART0_TYPE				PIO_PERIPH_A
#define PINS_UART0_ATTR				PIO_DEFAULT

/************************************************************************/
/*            Pins ADC                                                  */
/************************************************************************/



/************************************************************************/
/* LEDS                                                                 */
/************************************************************************/
//LED #0 pin definition (BLUE). 
#define LED_0_NAME					"blue LED D2"
#define PIN_LED_0					{PIO_PA19, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_0_MASK				PIO_PA19
#define PIN_LED_0_PIO				PIOA
#define PIN_LED_0_ID				ID_PIOA
#define PIN_LED_0_TYPE				PIO_OUTPUT_0
#define PIN_LED_0_ATTR				PIO_DEFAULT
#define LED0_GPIO					(PIO_PA19_IDX)
#define LED0_FLAGS					(PIO_OUTPUT_0 | PIO_DEFAULT)
#define LED0_ACTIVE_LEVEL			1

// LED #1 pin definition (GREEN)
#define LED_1_NAME					"green LED D3"
#define PIN_LED_1					{PIO_PA20, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_1_MASK				PIO_PA20
#define PIN_LED_1_PIO				PIOA
#define PIN_LED_1_ID				ID_PIOA
#define PIN_LED_1_TYPE				PIO_OUTPUT_0
#define PIN_LED_1_ATTR				PIO_DEFAULT
#define LED1_GPIO					(PIO_PA20_IDX)
#define LED1_FLAGS					(PIO_OUTPUT_0 | PIO_DEFAULT)
#define LED1_ACTIVE_LEVEL			1

/************************************************************************/
/* BUTTONS                                                              */
/************************************************************************/

/** Push button #0 definition. Attributes = pull-up(EXT) + debounce + interrupt on falling edge. */
#define PUSHBUTTON_1_NAME			"USRPB1"
#define GPIO_PUSH_BUTTON_1			(PIO_PB3_IDX)
#define GPIO_PUSH_BUTTON_1_FLAGS    (PIO_INPUT | PIO_DEBOUNCE | PIO_IT_FALL_EDGE )
#define PIN_PUSHBUTTON_1			{PIO_PB3, PIOB, ID_PIOB, PIO_INPUT, PIO_DEBOUNCE | PIO_IT_FALL_EDGE }
#define PIN_PUSHBUTTON_1_MASK		PIO_PB3
#define PIN_PUSHBUTTON_1_PIO		PIOB
#define PIN_PUSHBUTTON_1_ID			ID_PIOB
#define PIN_PUSHBUTTON_1_TYPE		PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR		PIO_DEBOUNCE | PIO_IT_FALL_EDGE 

/** Push button #1 definition. Attributes = pull-up + debounce + interrupt on falling edge. */
#define PUSHBUTTON_2_NAME			"USRPB2"
#define GPIO_PUSH_BUTTON_2			(PIO_PB4_IDX)
#define GPIO_PUSH_BUTTON_2_FLAGS    (PIO_INPUT |PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_PUSHBUTTON_2			{PIO_PB4_IDX, PIOC, ID_PIOB, PIO_INPUT, PIO_DEBOUNCE | PIO_IT_FALL_EDGE}
#define PIN_PUSHBUTTON_2_MASK		PIO_PB4_IDX
#define PIN_PUSHBUTTON_2_PIO		PIOB
#define PIN_PUSHBUTTON_2_ID			ID_PIOB
#define PIN_PUSHBUTTON_2_TYPE		PIO_INPUT
#define PIN_PUSHBUTTON_2_ATTR		PIO_DEBOUNCE | PIO_IT_FALL_EDGE

/** List of all push button definitions. */
#define PINS_PUSHBUTTONS    PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2







#endif // USER_BOARD_H
