/**
 * \file
 *
 * \brief SAM4S-EK Board Definition.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _USER_BOARD_H_
#define _USER_BOARD_H_

#include "compiler.h"
#include "system_sam4s.h"

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
#define BOARD_FREQ_SLCK_XTAL        (32768U)
#define BOARD_FREQ_SLCK_BYPASS      (32768U)
#define BOARD_FREQ_MAINCK_XTAL      (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS    (12000000U)

/** Master clock frequency */
#define BOARD_MCK                   CHIP_FREQ_CPU_MAX

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

//*******************************************************************************/
// LEDs
//*******************************************************************************/

/** LED #0 pin definition (GREEN). */
#define LED_0_NAME                 "LED_RED"
#define LED0_GPIO                  (PIO_PA15_IDX)
#define LED0_FLAGS                 (PIO_OUTPUT_0 | PIO_DEFAULT)
#define LED0_ACTIVE_LEVEL          1

/** LED #1 pin definition (GREEN). */
#define LED_1_NAME                 "LED_GREEN"
#define LED1_GPIO                  (PIO_PA16_IDX)
#define LED1_FLAGS                 (PIO_OUTPUT_0 | PIO_DEFAULT)
#define LED1_ACTIVE_LEVEL          1

#define BOARD_NUM_OF_LED           2

//*******************************************************************************/
// TESTPOINTs
//*******************************************************************************/

/** TP #0 pin definition. */
#define TP0_NAME                  "TESTPOINT_0"
#define TP0_GPIO                  (PIO_PB1_IDX)
#define TP0_FLAGS                 (PIO_OUTPUT_0 | PIO_DEFAULT)
#define TP0_ACTIVE_LEVEL          1

/** TP #1 pin definition. */
#define TP1_NAME                  "TESTPOINT_1"
#define TP1_GPIO                  (PIO_PA18_IDX)
#define TP1_FLAGS                 (PIO_OUTPUT_0 | PIO_DEFAULT)
#define TP1_ACTIVE_LEVEL          1

/** TP #2 pin definition. */
#define TP2_NAME                  "TESTPOINT_2"
#define TP2_GPIO                  (PIO_PA19_IDX)
#define TP2_FLAGS                 (PIO_OUTPUT_0 | PIO_DEFAULT)
#define TP2_ACTIVE_LEVEL          1

/** TP #3 pin definition. */
#define TP3_NAME                  "TESTPOINT_3"
#define TP3_GPIO                  (PIO_PA20_IDX)
#define TP3_FLAGS                 (PIO_OUTPUT_0 | PIO_DEFAULT)
#define TP3_ACTIVE_LEVEL          1

#define BOARD_NUM_OF_TP           4

//*******************************************************************************/
// PUSH BUTTON
//*******************************************************************************/

/** Push button #0 definition. Attributes = pull-up (ext) + debounce + interrupt on falling edge. */
#define PUSHBUTTON_1_NAME          "USRP_B1"
#define GPIO_PUSH_BUTTON_1         (PIO_PA11_IDX)
#define GPIO_PUSH_BUTTON_1_FLAGS   (PIO_INPUT | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_PUSHBUTTON_1           {PIO_PA11, PIOA, ID_PIOA, PIO_INPUT, PIO_DEBOUNCE | PIO_IT_FALL_EDGE}
#define PIN_PUSHBUTTON_1_MASK      PIO_PA11
#define PIN_PUSHBUTTON_1_PIO       PIOA
#define PIN_PUSHBUTTON_1_ID        ID_PIOA
#define PIN_PUSHBUTTON_1_TYPE      PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR      (PIO_DEBOUNCE | PIO_IT_FALL_EDGE)

/** Push button #1 definition. Attributes = pull-up (ext) + debounce + interrupt on falling edge. */
#define PUSHBUTTON_2_NAME          "USR_PB2"
#define GPIO_PUSH_BUTTON_2         (PIO_PA12_IDX)
#define GPIO_PUSH_BUTTON_2_FLAGS   (PIO_INPUT | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_PUSHBUTTON_2           {PIO_PA12, PIOA, ID_PIOA, PIO_INPUT, PIO_DEBOUNCE | PIO_IT_FALL_EDGE}
#define PIN_PUSHBUTTON_2_MASK      PIO_PA12
#define PIN_PUSHBUTTON_2_PIO       PIOA
#define PIN_PUSHBUTTON_2_ID        ID_PIOA
#define PIN_PUSHBUTTON_2_TYPE      PIO_INPUT
#define PIN_PUSHBUTTON_2_ATTR      (PIO_DEBOUNCE | PIO_IT_FALL_EDGE)

/** List of all push button definitions. */
#define PINS_PUSHBUTTONS           PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2

//*******************************************************************************/
// UART
//*******************************************************************************/

/** UART0 pins (UTXD0 and URXD0) definitions, PA9,10. */
#define PINS_UART0                 (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART0_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)
#define PINS_UART0_MASK            (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART0_PIO             PIOA
#define PINS_UART0_ID              ID_PIOA
#define PINS_UART0_TYPE            PIO_PERIPH_A
#define PINS_UART0_ATTR            PIO_DEFAULT

/** UART1 pins (UTXD1 and URXD1) definitions, PB2,PB3. */
#define PINS_UART1                 (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1)
#define PINS_UART1_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)
#define PINS_UART1_MASK            (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1)
#define PINS_UART1_PIO             PIOB
#define PINS_UART1_ID              ID_PIOB
#define PINS_UART1_TYPE            PIO_PERIPH_A
#define PINS_UART1_ATTR            PIO_DEFAULT

//*******************************************************************************/
// HSMCI for SD-Card
//*******************************************************************************/

/** HSMCI pins definition. */
/*! Number of slot connected on HSMCI interface */
#define SD_MMC_HSMCI_MEM_CNT       1
#define SD_MMC_HSMCI_SLOT_0_SIZE   4
#define PINS_HSMCI                 {0x3fUL << 26, PIOA, ID_PIOA, PIO_PERIPH_C, PIO_PULLUP}
/** HSMCI MCCDA pin definition. */
#define PIN_HSMCI_MCCDA_GPIO       (PIO_PA28_IDX)
#define PIN_HSMCI_MCCDA_FLAGS      (PIO_PERIPH_C | PIO_DEFAULT)
/** HSMCI MCCK pin definition. */
#define PIN_HSMCI_MCCK_GPIO        (PIO_PA29_IDX)
#define PIN_HSMCI_MCCK_FLAGS       (PIO_PERIPH_C | PIO_DEFAULT)
/** HSMCI MCDA0 pin definition. */
#define PIN_HSMCI_MCDA0_GPIO       (PIO_PA30_IDX)
#define PIN_HSMCI_MCDA0_FLAGS      (PIO_PERIPH_C | PIO_DEFAULT)
/** HSMCI MCDA1 pin definition. */
#define PIN_HSMCI_MCDA1_GPIO       (PIO_PA31_IDX)
#define PIN_HSMCI_MCDA1_FLAGS      (PIO_PERIPH_C | PIO_DEFAULT)
/** HSMCI MCDA2 pin definition. */
#define PIN_HSMCI_MCDA2_GPIO       (PIO_PA26_IDX)
#define PIN_HSMCI_MCDA2_FLAGS      (PIO_PERIPH_C | PIO_DEFAULT)
/** HSMCI MCDA3 pin definition. */
#define PIN_HSMCI_MCDA3_GPIO       (PIO_PA27_IDX)
#define PIN_HSMCI_MCDA3_FLAGS      (PIO_PERIPH_C | PIO_DEFAULT)
/** SD/MMC card detect pin definition. */
#define PIN_HSMCI_CD               {PIO_PA25, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP}
#define SD_MMC_0_CD_GPIO           (PIO_PA25_IDX)
#define SD_MMC_0_CD_PIO_ID         ID_PIOA
#define SD_MMC_0_CD_FLAGS          (PIO_INPUT | PIO_PULLUP)
#define SD_MMC_0_CD_DETECT_VALUE   0

/*----------------------------------------------------------------------------*/
/**
 * \page sam4s_ek_extcomp "SAM4S-EK - External components"
 * This page lists the definitions related to external on-board components
 * located in the board.h file for the SAM4S-EK.
 *
 * SD Card
 * - \ref BOARD_SD_PINS
 * - \ref BOARD_SD_PIN_CD
 *
 */

/** HSMCI pins that shall be configured to access the SD card. */
#define BOARD_SD_PINS              PINS_HSMCI
/** HSMCI Card Detect pin. */
#define BOARD_SD_PIN_CD            PIN_HSMCI_CD

#endif  // _USER_BOARD_H_
