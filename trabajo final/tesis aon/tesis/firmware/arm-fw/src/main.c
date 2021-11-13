/**
 * $Header
 * \file           main.c
 * \brief          main function.
 * \details
 * \author         Agustin Aon
 * \version        0.01
 * \date           21 July 2020
 * \details
 * \copyright
 *
 * \addtogroup main
 * \{
 */

/**************************** INCLUDES ***************************************/
#include "settings.h"

#include "apps/control.h"
#include "apps/signal_pwm.h"
#include "apps/system.h"
#include "apps_dsp/signal_adc.h"
#include "comm/comm.h"

/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief    Main function.    */
int main(void)
{
    // Initialization
    system_init();
    comm_init();
    control_init();

    // System ready
    system_ready();
    system_enable_interrupt();

    while(1)
    {
        comm_background();
        control_background();
    }
}

/**  @brief Handler for System Tick interrupt. */
void SysTick_Handler(void)
{
    if (control_is_enable())
    {
        signal_adc_start_conversion();
    }
}

/** @brief Handler for ADC. */
void ADC_Handler(void)
{
    signal_adc_process_data();
    control_update();
}

/** @brief Interrupt handler for the PWM controller. */
void PWM_Handler(void)
{
    /** PDC transfer packet */
    pdc_packet_t g_pdc_tx_packet;
    uint32_t pdc_status = pwm_get_interrupt_status(PWM);

    if ((pdc_status & PWM_PDC_TX_END) == PWM_PDC_TX_END)
    {
        /* Set up the PDC controller */
        g_pdc_tx_packet.ul_addr = (uint32_t)(control_get_pwm_duty_buf());
        g_pdc_tx_packet.ul_size = SETTINGS_PWM_BUF_LEN;

        /* Initialize the PDC transfer */
        pdc_tx_init(PDC_PWM, &g_pdc_tx_packet, 0);

        /* Send the PWM value */
        pdc_enable_transfer(PDC_PWM, PERIPH_PTCR_TXTEN);
    }
}

void overcurrent_isr(void)
{
    //signal_pwm_enable(true);        // deshabilitado porque levanta ruido
    drv1_off();
    drv2_off();
}

/** @brief Handler for UART 1. */
void UART1_Handler(void)
{
    comm_preprocess_uart_PC_RX();
}
