/**
 * $Header
 * @file           usb.c
 * @brief          usb function.
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
#include "usb.h"
#include "apps/system.h"
#include "comm/comm.h"


/**************************** LOCAL VARIABLES ********************************/
static volatile bool b_cdc_enable = false;


/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief Called when USB connected */
bool my_callback_cdc_enable(uint8_t port)
{
    b_cdc_enable = true;
    return true;
}

/** @brief Called when USB disconnected */
void my_callback_cdc_disable(uint8_t port)
{
    b_cdc_enable = false;
}

/** @brief Called when USB received data */
void my_callback_cdc_rx_notify(uint8_t port)
{
    if (b_cdc_enable)
    {
        comm_preprocess_usb_PC_RX();
    }
}

/** @brief Called when USB connected and recognized by host */
void my_callback_cdc_set_dtr(uint8_t port, bool b_enable)
{
    if (b_enable)
    {
        led_green_on();
    }
    else
    {
        led_green_off();
    }
}