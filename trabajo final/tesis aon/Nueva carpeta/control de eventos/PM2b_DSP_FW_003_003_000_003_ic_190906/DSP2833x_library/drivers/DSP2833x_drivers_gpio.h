// $Header
/// @file			DSP2833x_drivers_gpio.h
/// @brief  		General Purpose Input Output drivers.
/// @details
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		27 November 2014
/// @details
/// @copyright
///

#ifndef _DSP2833X_DRIVERS_GPIO_H_
#define _DSP2833X_DRIVERS_GPIO_H_

/**************************** INCLUDES ***************************************/

#include "DSP2833x_library.h"

/**************************** MACROS *****************************************/

	// GPIO CONTROL REGISTERS (EALLOW PROTECTED)
	#define GPACTRL_R      (*((volatile Uint32 *)0x6F80))
	#define GPAQSEL1_R     (*((volatile Uint32 *)0x6F82))
	#define GPAQSEL2_R     (*((volatile Uint32 *)0x6F84))
	#define GPAMUX1_R      (*((volatile Uint32 *)0x6F86))
	#define GPAMUX2_R      (*((volatile Uint32 *)0x6F88))
	#define GPADIR_R       (*((volatile Uint32 *)0x6F8A))
	#define GPAPUD_R       (*((volatile Uint32 *)0x6F8C))
	#define GPBCTRL_R      (*((volatile Uint32 *)0x6F90))
	#define GPBQSEL1_R     (*((volatile Uint32 *)0x6F92))
	#define GPBQSEL2_R     (*((volatile Uint32 *)0x6F94))
	#define GPBMUX1_R      (*((volatile Uint32 *)0x6F96))
	#define GPBMUX2_R      (*((volatile Uint32 *)0x6F98))
	#define GPBDIR_R       (*((volatile Uint32 *)0x6F9A))
	#define GPBPUD_R       (*((volatile Uint32 *)0x6F9C))
	#define GPCMUX1_R      (*((volatile Uint32 *)0x6FA6))
	#define GPCMUX2_R      (*((volatile Uint32 *)0x6FA8))
	#define GPCDIR_R       (*((volatile Uint32 *)0x6FAA))
	#define GPCPUD_R       (*((volatile Uint32 *)0x6FAC))

	// GPIO DATA REGISTERS (NOT EALLOW PROTECTED)
	#define GPADAT_R       (*((volatile Uint32 *)0x6FC0))
	#define GPASET_R       (*((volatile Uint32 *)0x6FC2))
	#define GPACLEAR_R     (*((volatile Uint32 *)0x6FC4))
	#define GPATOGGLE_R    (*((volatile Uint32 *)0x6FC6))
	#define GPBDAT_R       (*((volatile Uint32 *)0x6FC8))
	#define GPBSET_R       (*((volatile Uint32 *)0x6FCA))
	#define GPBCLEAR_R     (*((volatile Uint32 *)0x6FCC))
	#define GPBTOGGLE_R    (*((volatile Uint32 *)0x6FCE))
	#define GPCDAT_R       (*((volatile Uint32 *)0x6FD0))
	#define GPCSET_R       (*((volatile Uint32 *)0x6FD2))
	#define GPCCLEAR_R     (*((volatile Uint32 *)0x6FD4))
	#define GPCTOGGLE_R    (*((volatile Uint32 *)0x6FD6))

	// GPIO A BIT LOCATION INTO REGISTER
	#define GPIOA00   (0)
	#define GPIOA01   (1)
	#define GPIOA02   (2)
	#define GPIOA03   (3)
	#define GPIOA04   (4)
	#define GPIOA05   (5)
	#define GPIOA06   (6)
	#define GPIOA07   (7)
	#define GPIOA08   (8)
	#define GPIOA09   (9)
	#define GPIOA10   (10)
	#define GPIOA11   (11)
	#define GPIOA12   (12)
	#define GPIOA13   (13)
	#define GPIOA14   (14)
	#define GPIOA15   (15)
	#define GPIOA16   (16)
	#define GPIOA17   (17)
	#define GPIOA18   (18)
	#define GPIOA19   (19)
	#define GPIOA20   (20)
	#define GPIOA21   (21)
	#define GPIOA22   (22)
	#define GPIOA23   (23)
	#define GPIOA24   (24)
	#define GPIOA25   (25)
	#define GPIOA26   (26)
	#define GPIOA27   (27)
	#define GPIOA28   (28)
	#define GPIOA29   (29)
	#define GPIOA30   (30)
	#define GPIOA31   (31)
	// GPIO B BIT LOCATION INTO REGISTER
	#define GPIOB32   (GPIOA00)
	#define GPIOB33   (GPIOA01)
	#define GPIOB34   (GPIOA02)
	#define GPIOB35   (GPIOA03)
	#define GPIOB36   (GPIOA04)
	#define GPIOB37   (GPIOA05)
	#define GPIOB38   (GPIOA06)
	#define GPIOB39   (GPIOA07)
	#define GPIOB40   (GPIOA08)
	#define GPIOB41   (GPIOA09)
	#define GPIOB42   (GPIOA10)
	#define GPIOB43   (GPIOA11)
	#define GPIOB44   (GPIOA12)
	#define GPIOB45   (GPIOA13)
	#define GPIOB46   (GPIOA14)
	#define GPIOB47   (GPIOA15)
	#define GPIOB48   (GPIOA16)
	#define GPIOB49   (GPIOA17)
	#define GPIOB50   (GPIOA18)
	#define GPIOB51   (GPIOA19)
	#define GPIOB52   (GPIOA20)
	#define GPIOB53   (GPIOA21)
	#define GPIOB54   (GPIOA22)
	#define GPIOB55   (GPIOA23)
	#define GPIOB56   (GPIOA24)
	#define GPIOB57   (GPIOA25)
	#define GPIOB58   (GPIOA26)
	#define GPIOB59   (GPIOA27)
	#define GPIOB60   (GPIOA28)
	#define GPIOB61   (GPIOA29)
	#define GPIOB62   (GPIOA30)
	#define GPIOB63   (GPIOA31)
	// GPIO C BIT LOCATION INTO REGISTER
	#define GPIOC64   (GPIOA00)
	#define GPIOC65   (GPIOA01)
	#define GPIOC66   (GPIOA02)
	#define GPIOC67   (GPIOA03)
	#define GPIOC68   (GPIOA04)
	#define GPIOC69   (GPIOA05)
	#define GPIOC70   (GPIOA06)
	#define GPIOC71   (GPIOA07)
	#define GPIOC72   (GPIOA08)
	#define GPIOC73   (GPIOA09)
	#define GPIOC74   (GPIOA10)
	#define GPIOC75   (GPIOA11)
	#define GPIOC76   (GPIOA12)
	#define GPIOC77   (GPIOA13)
	#define GPIOC78   (GPIOA14)
	#define GPIOC79   (GPIOA15)
	#define GPIOC80   (GPIOA16)
	#define GPIOC81   (GPIOA17)
	#define GPIOC82   (GPIOA18)
	#define GPIOC83   (GPIOA19)
	#define GPIOC84   (GPIOA20)
	#define GPIOC85   (GPIOA21)
	#define GPIOC86   (GPIOA22)
	#define GPIOC87   (GPIOA23)
	// GPIO 16 BIT LOCATION INTO REGISTER
	#define GPIOX16L  (0)
	#define GPIOX16H  (16)
	// GPIO 32 BIT LOCATION INTO REGISTER
	#define GPIOX32   (0)

	// GPIO A BITS MASK
	#define GPIOA00_M   ((Uint32)1u<<0)
	#define GPIOA01_M   ((Uint32)1u<<1)
	#define GPIOA02_M   ((Uint32)1u<<2)
	#define GPIOA03_M   ((Uint32)1u<<3)
	#define GPIOA04_M   ((Uint32)1u<<4)
	#define GPIOA05_M   ((Uint32)1u<<5)
	#define GPIOA06_M   ((Uint32)1u<<6)
	#define GPIOA07_M   ((Uint32)1u<<7)
	#define GPIOA08_M   ((Uint32)1u<<8)
	#define GPIOA09_M   ((Uint32)1u<<9)
	#define GPIOA10_M   ((Uint32)1u<<10)
	#define GPIOA11_M   ((Uint32)1u<<11)
	#define GPIOA12_M   ((Uint32)1u<<12)
	#define GPIOA13_M   ((Uint32)1u<<13)
	#define GPIOA14_M   ((Uint32)1u<<14)
	#define GPIOA15_M   ((Uint32)1u<<15)
	#define GPIOA16_M   ((Uint32)1u<<16)
	#define GPIOA17_M   ((Uint32)1u<<17)
	#define GPIOA18_M   ((Uint32)1u<<18)
	#define GPIOA19_M   ((Uint32)1u<<19)
	#define GPIOA20_M   ((Uint32)1u<<20)
	#define GPIOA21_M   ((Uint32)1u<<21)
	#define GPIOA22_M   ((Uint32)1u<<22)
	#define GPIOA23_M   ((Uint32)1u<<23)
	#define GPIOA24_M   ((Uint32)1u<<24)
	#define GPIOA25_M   ((Uint32)1u<<25)
	#define GPIOA26_M   ((Uint32)1u<<26)
	#define GPIOA27_M   ((Uint32)1u<<27)
	#define GPIOA28_M   ((Uint32)1u<<28)
	#define GPIOA29_M   ((Uint32)1u<<29)
	#define GPIOA30_M   ((Uint32)1u<<30)
	#define GPIOA31_M   ((Uint32)1u<<31)
	// GPIO B BITS MASK
	#define GPIOB32_M   (GPIOA00_M)
	#define GPIOB33_M   (GPIOA01_M)
	#define GPIOB34_M   (GPIOA02_M)
	#define GPIOB35_M   (GPIOA03_M)
	#define GPIOB36_M   (GPIOA04_M)
	#define GPIOB37_M   (GPIOA05_M)
	#define GPIOB38_M   (GPIOA06_M)
	#define GPIOB39_M   (GPIOA07_M)
	#define GPIOB40_M   (GPIOA08_M)
	#define GPIOB41_M   (GPIOA09_M)
	#define GPIOB42_M   (GPIOA10_M)
	#define GPIOB43_M   (GPIOA11_M)
	#define GPIOB44_M   (GPIOA12_M)
	#define GPIOB45_M   (GPIOA13_M)
	#define GPIOB46_M   (GPIOA14_M)
	#define GPIOB47_M   (GPIOA15_M)
	#define GPIOB48_M   (GPIOA16_M)
	#define GPIOB49_M   (GPIOA17_M)
	#define GPIOB50_M   (GPIOA18_M)
	#define GPIOB51_M   (GPIOA19_M)
	#define GPIOB52_M   (GPIOA20_M)
	#define GPIOB53_M   (GPIOA21_M)
	#define GPIOB54_M   (GPIOA22_M)
	#define GPIOB55_M   (GPIOA23_M)
	#define GPIOB56_M   (GPIOA24_M)
	#define GPIOB57_M   (GPIOA25_M)
	#define GPIOB58_M   (GPIOA26_M)
	#define GPIOB59_M   (GPIOA27_M)
	#define GPIOB60_M   (GPIOA28_M)
	#define GPIOB61_M   (GPIOA29_M)
	#define GPIOB62_M   (GPIOA30_M)
	#define GPIOB63_M   (GPIOA31_M)
	// GPIO C BITS MASK
	#define GPIOC64_M   (GPIOA00_M)
	#define GPIOC65_M   (GPIOA01_M)
	#define GPIOC66_M   (GPIOA02_M)
	#define GPIOC67_M   (GPIOA03_M)
	#define GPIOC68_M   (GPIOA04_M)
	#define GPIOC69_M   (GPIOA05_M)
	#define GPIOC70_M   (GPIOA06_M)
	#define GPIOC71_M   (GPIOA07_M)
	#define GPIOC72_M   (GPIOA08_M)
	#define GPIOC73_M   (GPIOA09_M)
	#define GPIOC74_M   (GPIOA10_M)
	#define GPIOC75_M   (GPIOA11_M)
	#define GPIOC76_M   (GPIOA12_M)
	#define GPIOC77_M   (GPIOA13_M)
	#define GPIOC78_M   (GPIOA14_M)
	#define GPIOC79_M   (GPIOA15_M)
	#define GPIOC80_M   (GPIOA16_M)
	#define GPIOC81_M   (GPIOA17_M)
	#define GPIOC82_M   (GPIOA18_M)
	#define GPIOC83_M   (GPIOA19_M)
	#define GPIOC84_M   (GPIOA20_M)
	#define GPIOC85_M   (GPIOA21_M)
	#define GPIOC86_M   (GPIOA22_M)
	#define GPIOC87_M   (GPIOA23_M)
	// GPIO 16 BITS MASK
	#define GPIOX16LSB_M  ((Uint32)0x0000FFFF)
	#define GPIOX16MSB_M  ((Uint32)0xFFFF0000)
	// GPIO 32 BITS MASK
	#define GPIOX32_M   ((Uint32)0xFFFFFFFF)

// Macros GPIO A
	#define GPIOA_OUT_SET_BIT(MASK)     GPASET_R = MASK
	#define GPIOA_OUT_CLR_BIT(MASK)     GPACLEAR_R = MASK
	#define GPIOA_OUT_TOG_BIT(MASK)     GPATOGGLE_R = MASK
	#define GPIOA_OUT_DAT_BIT(MASK,x)   { if (x) {GPIOA_OUT_SET_BIT(MASK);} else {GPIOA_OUT_CLR_BIT(MASK);} }
	#define GPIOA_OUT_DAT_16LSB(data)   GPADAT_R = (  (GPIOX16LSB_M & data)      | (GPIOX16MSB_M & GPADAT_R) )
	#define GPIOA_OUT_DAT_16MSB(data)   GPADAT_R = ( ((GPIOX16LSB_M & data)<<16) | (GPIOX16LSB_M & GPADAT_R) )
	#define GPIOA_OUT_DAT_32BIT(data)   GPADAT_R = ((Uint32)data)
	#define GPIOA_OUT_DIR_16LSB(data)   GPADIR_R = (  (GPIOX16LSB_M & data)      | (GPIOX16MSB_M & GPADIR_R) )
	#define GPIOA_READ_DAT_BIT(MASK)    (GPADAT_R & MASK)
	#define GPIOA_READ_DAT_16LSB()      (GPIOX16LSB_M & GPADAT_R)
	#define GPIOA_READ_DAT_16MSB()      ((GPIOX16MSB_M & GPADAT_R)>>16)
	#define GPIOA_READ_DAT_32BIT()      (GPADAT_R)

// Macros GPIO B
	#define GPIOB_OUT_SET_BIT(MASK)     GPBSET_R = MASK
	#define GPIOB_OUT_CLR_BIT(MASK)     GPBCLEAR_R = MASK
	#define GPIOB_OUT_TOG_BIT(MASK)     GPBTOGGLE_R = MASK

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

/// @endcond

#endif  // end of _DSP2833X_DRIVERS_GPIO_H_ definition

//#################################### FIN #################################
















  




















