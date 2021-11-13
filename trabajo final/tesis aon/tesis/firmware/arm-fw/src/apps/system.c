/**
 * $Header
 * @file           system.c
 * @brief          System apps
 * @details
 * @author         Agustin Aon
 * @version        0.01
 * @date           21 July 2020
 * @details
 * @copyright
 *
 * @addtogroup main
 * @{
 */

/**************************** INCLUDES ***************************************/
#include "system.h"
#include "apps_dsp/signal_adc.h"
#include "signal_pwm.h"


/**************************** LOCAL FUNCTIONS ********************************/

/** @brief Configure LED pins  */
static void arm_leds_init(void)
{
    // led 0
    gpio_configure_pin(LED0_GPIO, LED0_FLAGS);
    gpio_set_pin_low(LED0_GPIO);                 // initial value

    // led 1
    gpio_configure_pin(LED1_GPIO, LED1_FLAGS);
    gpio_set_pin_low(LED1_GPIO);                 // initial value
}

/** @brief GPIO initialization  */
static void arm_drv_gpio_init(void)
{
    gpio_configure_pin(DRV1_SD_PIN, PIO_OUTPUT_1 | PIO_DEFAULT);
    gpio_set_pin_high(DRV1_SD_PIN);

    gpio_configure_pin(DRV2_SD_PIN, PIO_OUTPUT_1 | PIO_DEFAULT);
    gpio_set_pin_high(DRV2_SD_PIN);
}

/** @brief UARTs initialization */
static void arm_uart_init(void)
{
    sam_uart_opt_t uart_settings;
    uart_settings.ul_mck = sysclk_get_peripheral_hz();
    uart_settings.ul_baudrate = SETTINGS_COMM_ARM_BAUDRATE;
    uart_settings.ul_mode = US_MR_CHRL_8_BIT |      // 8 bit lenght
                            US_MR_PAR_NO |          // no parity
                            US_MR_NBSTOP_1_BIT;     // 1 stop bit

    /*
    // Configure UART0 pins
    gpio_configure_group(PINS_UART0_PIO, PINS_UART0, PINS_UART0_FLAGS);
    // enable uart clk
    sysclk_enable_peripheral_clock(ID_UART0);
    // Configure UART
    uart_init( UART0 , &uart_settings);
    uart_enable( UART0 );
    uart_enable_interrupt( UART0 , UART_IER_RXRDY );
    */

    // Configure UART1 pins
    gpio_configure_group(PINS_UART1_PIO, PINS_UART1, PINS_UART1_FLAGS);
    // enable uart clk
    sysclk_enable_peripheral_clock(ID_UART1);
    // Configure UART
    uart_init( UART1 , &uart_settings);
    uart_enable( UART1 );
    uart_enable_interrupt( UART1 , UART_IER_RXRDY );
}

/** @brief Toggle LEDs 3 times */
static void arm_toggle_leds_3_times(void)
{
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

/** @brief USB initialization */
static void arm_usb_init(void)
{
    udc_start();
}

/** @brief ADC initialization */
static void arm_adc_init(void)
{
    signal_adc_init();
}

/** @brief PWM initialization */
static void arm_pwm_init(void)
{
    // Overcurrent detector
    // gpio_configure_pin(PIO_PA9_IDX, PIO_INPUT | PIO_IT_RISE_EDGE);
    // pio_handler_set(PIOA, ID_PIOA, PIO_PA9, PIO_IT_RISE_EDGE, overcurrent_isr);
    // pio_enable_interrupt(PIOA, PIO_PA9);

    // PWMs
    signal_pwm_init();
}


/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief Initialize system */
void system_init()
{
    // general initialization
    WDT->WDT_MR = WDT_MR_WDDIS; // Disable the watchdog
    sysclk_init();
    sysclk_enable_peripheral_clock(ID_PIOA);
    sysclk_enable_peripheral_clock(ID_PIOB);
    gpio_configure_group( PIOA , 0xFFFFFFFF , (PIO_INPUT | PIO_DEFAULT));
    gpio_configure_group( PIOB , 0xFFFFFFFF , (PIO_INPUT | PIO_DEFAULT));
    pio_pull_down( PIOA , 0xFFFFFFFF , 0 );
    pio_pull_down( PIOB , 0xFFFFFFFF , 0 );
    pio_pull_up( PIOA , 0xFFFFFFFF , 0 );
    pio_pull_up( PIOB , 0xFFFFFFFF , 0 );

    // specific initialization
    arm_leds_init();
    arm_drv_gpio_init();
    arm_uart_init();
    arm_adc_init();
    arm_pwm_init();
    arm_usb_init();
}

/** @brief Show that system is ready */
void system_ready()
{
    arm_toggle_leds_3_times();
}

/** @brief Enable interrupts. */
void system_enable_interrupt(void)
{
    // Enable systick
    SysTick_Config(sysclk_get_cpu_hz() / SETTINGS_ARM_TIMER_FREQ_HZ);

    // Enable UART 1
    NVIC_DisableIRQ((IRQn_Type) UART1_IRQn);
    NVIC_ClearPendingIRQ((IRQn_Type) UART1_IRQn);
    NVIC_SetPriority((IRQn_Type) UART1_IRQn,2);
    NVIC_EnableIRQ((IRQn_Type) UART1_IRQn);

    // Enable ADC
    NVIC_DisableIRQ((IRQn_Type) ADC_IRQn);
    NVIC_ClearPendingIRQ((IRQn_Type) ADC_IRQn);
    NVIC_SetPriority((IRQn_Type) ADC_IRQn,1);
    NVIC_EnableIRQ((IRQn_Type) ADC_IRQn);

    // Enable overcurrent ISR
    NVIC_DisableIRQ((IRQn_Type)ID_PIOA);
    NVIC_ClearPendingIRQ((IRQn_Type) ID_PIOA);
    NVIC_SetPriority((IRQn_Type) ID_PIOA,3);
    NVIC_EnableIRQ((IRQn_Type) ID_PIOA);

    // Enable PWM
    NVIC_DisableIRQ(PWM_IRQn);
    NVIC_ClearPendingIRQ(PWM_IRQn);
    NVIC_SetPriority(PWM_IRQn, 4);
    NVIC_EnableIRQ(PWM_IRQn);

}

/** @brief Disable interrupts. */
void system_disable_interrupt(void)
{
    NVIC_DisableIRQ((IRQn_Type) UART1_IRQn);
    NVIC_DisableIRQ((IRQn_Type) ADC_IRQn);
}