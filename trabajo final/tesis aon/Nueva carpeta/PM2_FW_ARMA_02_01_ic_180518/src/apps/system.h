// $Header
/// @file			system.h
/// @brief  		System apps.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		22 December 2016
/// @details
/// @copyright
///

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"

/**************************** MACROS *****************************************/

#define led_red_on()      gpio_set_pin_high(LED0_GPIO)
#define led_red_off()     gpio_set_pin_low(LED0_GPIO)
#define led_green_on()    gpio_set_pin_high(LED1_GPIO)
#define led_green_off()   gpio_set_pin_low(LED1_GPIO)

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

// interrupt service routines
void coreio_push_button_1_isr(uint32_t id, uint32_t mask);
void coreio_push_button_2_isr(uint32_t id, uint32_t mask);

// system functions
void system_init(void);
void system_ready(void);
void system_enable_interrupt(void);
void system_disable_interrupt(void);

// ARM
void system_reset_ARM(void);

/// @endcond

#endif // __SYSTEM_H__

