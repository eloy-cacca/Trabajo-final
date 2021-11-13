/** $Header
  * \file            system.h
  * \brief          System apps
  * \details
  * \author         Agustin Aon
  * \version           0.1
  * \date              21 July 2020
  * \details
  * \copyright
  */
#ifndef SYSTEM_H_

#define SYSTEM_H_

/**************************** INCLUDES ***************************************/
#include "settings.h"


/**************************** MACROS *****************************************/
#define led_red_on()        gpio_set_pin_high(LED0_GPIO)
#define led_red_off()       gpio_set_pin_low(LED0_GPIO)
#define led_green_on()      gpio_set_pin_high(LED1_GPIO)
#define led_green_off()     gpio_set_pin_low(LED1_GPIO)

#define DRV1_SD_PIN         (PIO_PA29_IDX)
#define DRV2_SD_PIN         (PIO_PA1_IDX)

#define drv1_on()           gpio_set_pin_low(DRV1_SD_PIN)
#define drv1_off()          gpio_set_pin_high(DRV1_SD_PIN)
#define drv2_on()           gpio_set_pin_low(DRV2_SD_PIN)
#define drv2_off()          gpio_set_pin_high(DRV2_SD_PIN)


/**************************** FUNCTIONS **************************************/
void system_init(void);
void system_ready(void);
void system_enable_interrupt(void);
void system_disable_interrupt(void);
void overcurrent_isr(void);  // overcurrent interruption routine


#endif /* SYSTEM_H_ */