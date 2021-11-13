// $Header
/// @file			system.h
/// @brief  		DSP module.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		29 February 2016
/// @details
/// @copyright
///

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/**************************** INCLUDES ***************************************/

#include "settings.h"

//*****************************************************************************
// The following functions are located in "ramfuncs", so they always run from
// RAM and must be relocated when the flash initialization is executed. */
//*****************************************************************************
//#pragma CODE_SECTION(cputimer0_irq, "ramfuncs");

/**************************** MACROS *****************************************/

/* General-Purpose Input/Output board definition */
#define METER_CTRL_RPI_TURNOFF  GPIOA00_M	// GPIO0  / EPWM1A
#define METER_CTRL_RPI_SUPPLY   GPIOA01_M	// GPIO1  / EPWM1B / ECAP6 / MFSRB
#define DAC1			        GPIOA02_M	// GPIO2  / EPWM2A
#define DSP_TESTPOINT_04        GPIOA03_M	// GPIO3  / EPWM2B / ECAP5 / MCLKRB
#define DAC2    				GPIOA04_M	// GPIO4  / EPWM3A
//#define GPIO_GMB_19_D05       GPIOA05_M 	// GPIO5  / EPWM3B / MFSRA / ECAP1
//#define GPIO_GMB_18_D06       GPIOA06_M	// GPIO6  / EPWM4A / EPWMSYNCI / EPWMSYNCO
//#define GPIO_GMB_17_D07       GPIOA07_M	// GPIO7  / EPWM4B / MCLKRA / ECAP2
//#define GPIO_GMB_16_D08       GPIOA08_M 	// GPIO8  / EPWM5A / CANTXB / ADCSOCAO
//#define GPIO_GMB_15_D09       GPIOA09_M	// GPIO9  / EPWM5B / SCITXDB / ECAP3
//#define GPIO_GMB_14_D10       GPIOA10_M 	// GPIO10 / EPWM6A / CANRXB / ADCSOCBO
//#define GPIO_GMB_13_D11       GPIOA11_M 	// GPIO11 / EPWM6B / SCIRXDB / ECAP4
//#define GPIO_GMB_12_D12       GPIOA12_M 	// GPIO12 / TZ1 / CANTXB / MDXB
//#define GPIO_GMB_11_D13       GPIOA13_M 	// GPIO13 / TZ2 / CANRXB / MDRB
//#define GPIO_GMB_10_D14       GPIOA14_M	// GPIO14 / TZ3/XHOLD / SCITXDB / MCLKXB			<- SCI-B
//#define GPIO_GMB_09_D15       GPIOA15_M	// GPIO15 / TZ4/XHOLDA / SCIRXDB / MFSXB			<- SCI-B
//#define GPIO_GMB_31_DAC_SDIN  GPIOA16_M	// GPIO16 / SPISIMOA / CANTXB / TZ5					<- SPI
//#define GPIO_GMB_32_DAC_LOAD	GPIOA17_M	// GPIO17 / SPISOMIA / CANRXB / TZ6					<- SPI
//#define GPIO_GMB_30_DAC_SCLK	GPIOA18_M 	// GPIO18 / SPICLKA / SCITXDB / CANRXA				<- SPI
//#define GPIO_GMB_29_DAC_SYNC	GPIOA19_M	// GPIO19 / SPISTEA / SCIRXDB / CANTXA 				<- SPI
//#define GPIO_GMB_28_ADC_SOCD	GPIOA20_M	// GPIO20 / EQEP1A / MDXA / CANTXB
#define DSP_TESTPOINT_03    	GPIOA21_M	// GPIO21 / EQEP1B / MDRA / CANRXB
#define METER_CTRL_BATTERY_EN   GPIOA22_M	// GPIO22 / EQEP1S / MCLKXA / SCITXDB
//#define GPIO_GMB_25_ADC_SOCA	GPIOA23_M	// GPIO23 / EQEP1I / MFSXA /SCIRXDB
//#define GPIO_GMB_08_ADC_CS    GPIOA24_M
//#define GPIO_GMB_07_ADC_RD    GPIOA25_M
//#define GPIO_GMB_06_ARM_CMD   GPIOA26_M
//#define DSP_TESTPOINT_03      GPIOA27_M
//#define GPIO_DSP_SCIA_RX		GPIOA28_M
//#define GPIO_DSP_SCIA_TX		GPIOA29_M
// #define						GPIOA30_M
#define DSP_LED_RED_01			GPIOA31_M
#define DSP_TESTPOINT_01		GPIOB32_M	// I2C-SDA
#define DSP_TESTPOINT_02		GPIOB33_M	// I2C-SCL
#define DSP_LED_RED_02			GPIOB34_M
// NO DISPONIBLE				GPIO35_M
// NO DISPONIBLE				GPIO36_M
// NO DISPONIBLE				GPIO37_M
// NO DISPONIBLE				GPIO38_M
// NO DISPONIBLE				GPIO39_M
// NO DISPONIBLE				GPIO40_M
// NO DISPONIBLE				GPIO41_M
// NO DISPONIBLE				GPIO42_M
// NO DISPONIBLE				GPIO43_M
// NO DISPONIBLE				GPIO44_M
// NO DISPONIBLE				GPIO45_M
// NO DISPONIBLE				GPIO46_M
// NO DISPONIBLE				GPIO47_M
// #define						GPIO48_M
// #define						GPIO49_M
// NO DISPONIBLE				GPIO50_M
// NO DISPONIBLE				GPIO51_M
// NO DISPONIBLE				GPIO52_M
// NO DISPONIBLE				GPIO53_M
// NO DISPONIBLE				GPIO54_M
// NO DISPONIBLE				GPIO55_M
// NO DISPONIBLE				GPIO56_M
// NO DISPONIBLE				GPIO57_M
// #define						GPIO58_M
// #define						GPIO59_M
// #define						GPIO60_M
// #define						GPIO61_M
// #define						GPIO62_M	// SCIC-RX
// #define						GPIO63_M	// SCIC-TX
// NO DISPONIBLE				GPIO64_M
// NO DISPONIBLE				GPIO65_M
// NO DISPONIBLE				GPIO66_M
// NO DISPONIBLE				GPIO67_M
// NO DISPONIBLE				GPIO68_M
// NO DISPONIBLE				GPIO69_M
// NO DISPONIBLE				GPIO70_M
// NO DISPONIBLE				GPIO71_M
// NO DISPONIBLE				GPIO72_M
// NO DISPONIBLE				GPIO73_M
// NO DISPONIBLE				GPIO74_M
// NO DISPONIBLE				GPIO75_M
// NO DISPONIBLE				GPIO76_M
// NO DISPONIBLE				GPIO77_M
// NO DISPONIBLE				GPIO78_M
// NO DISPONIBLE				GPIO79_M
// NO DISPONIBLE				GPIO80_M
// NO DISPONIBLE				GPIO81_M
// NO DISPONIBLE				GPIO82_M
// NO DISPONIBLE				GPIO83_M
//#define DSP_BOOTMODE_A        GPIO84_M	// controlCARD
//#define DSP_BOOTMODE_B        GPIO85_M	// controlCARD
//#define DSP_BOOTMODE_C        GPIO86_M	// controlCARD
//#define DSP_BOOTMODE_D        GPIO87_M	// controlCARD


/* test point 01 */
#define test_point_01_set()          GPIOB_OUT_SET_BIT(DSP_TESTPOINT_01)
#define test_point_01_clr()          GPIOB_OUT_CLR_BIT(DSP_TESTPOINT_01)

/* test point 02 */
#define test_point_02_set()          GPIOB_OUT_SET_BIT(DSP_TESTPOINT_02)
#define test_point_02_clr()          GPIOB_OUT_CLR_BIT(DSP_TESTPOINT_02)

/* test point 03 */
#define test_point_03_set()          GPIOA_OUT_SET_BIT(DSP_TESTPOINT_03)
#define test_point_03_clr()          GPIOA_OUT_CLR_BIT(DSP_TESTPOINT_03)

/* test point 04 */
#define test_point_04_set()          GPIOA_OUT_SET_BIT(DSP_TESTPOINT_04)
#define test_point_04_clr()          GPIOA_OUT_CLR_BIT(DSP_TESTPOINT_04)

/* DSP red led 01 (used to show cputimer0 service routine time utilization) */
#define dsp_red_led_control_on()     GPIOA_OUT_CLR_BIT(DSP_LED_RED_01)
#define dsp_red_led_control_off()    GPIOA_OUT_SET_BIT(DSP_LED_RED_01)
#define dsp_red_led_control_tog()    GPIOA_OUT_TOG_BIT(DSP_LED_RED_01)

/* DSP red led 02 (used to show communication pending) */
#define dsp_red_led_comm_on()        GPIOB_OUT_CLR_BIT(DSP_LED_RED_02)
#define dsp_red_led_comm_off()       GPIOB_OUT_SET_BIT(DSP_LED_RED_02)
#define dsp_red_led_comm_tog()       GPIOB_OUT_TOG_BIT(DSP_LED_RED_02)

/* Meter Control */
#define meter_ctrl_battery_enable()      GPIOA_OUT_SET_BIT(METER_CTRL_BATTERY_EN)
#define meter_ctrl_battery_disable()     GPIOA_OUT_CLR_BIT(METER_CTRL_BATTERY_EN)
#define meter_ctrl_rpi_turn_on()         GPIOA_OUT_SET_BIT(METER_CTRL_RPI_TURNOFF)
#define meter_ctrl_rpi_turn_off()        GPIOA_OUT_CLR_BIT(METER_CTRL_RPI_TURNOFF)
#define meter_ctrl_rpi_supply_enable()   GPIOA_OUT_SET_BIT(METER_CTRL_RPI_SUPPLY)
#define meter_ctrl_rpi_supply_disable()  GPIOA_OUT_CLR_BIT(METER_CTRL_RPI_SUPPLY)

/* cputimer0 commands */
#define cputimer0_set_period         CpuTimer0Regs.PRD.all

/* cputimer1 commands */
#define cputimer1_timer_register     CpuTimer1Regs.TIM.all

/* epwm1 commands */
#define epwm1_set_period			 EPwm1Regs.TBPRD
#define epwm1_start0()				 EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP

/* dac 1 (EPWM2A / GPIO02) */
#define dac1_16bits_signed(x)		 EPwm2Regs.CMPA.half.CMPA = (uint16_t)(0x7FFF + x)
#define dac1_16bits_unsigned(x)		 EPwm2Regs.CMPA.half.CMPA = (uint16_t)x

/* dac 2 (EPWM3A / GPIO04) */
#define dac2_16bits_signed(x)		 EPwm3Regs.CMPA.half.CMPA = (uint16_t)(0x7FFF + x)
#define dac2_16bits_unsigned(x)		 EPwm3Regs.CMPA.half.CMPA = (uint16_t)x

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

interrupt void cputimer0_irq(void); // function located in main.c
interrupt void sci_a_rx_irq(void); // function located in main.c
interrupt void sci_b_rx_irq(void); // function located in main.c
interrupt void sci_c_rx_irq(void); // function located in main.c
interrupt void adc_isr(void); // function located in main.c

void system_init(void);
void system_ready(void);

/// @endcond

#endif // __SYSTEM_H__

