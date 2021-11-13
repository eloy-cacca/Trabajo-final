// $Header
/// @file		    DSP28335_drivers_initDefault.c
/// @brief 			Default Initialization.
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		26 November 2014
/// @details
/// @copyright
///

/// @addtogroup
/// @{

/**************************** INCLUDES ***************************************/

#include "DSP2833x_library.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/*************************** FUNCTION PROTOTYPES *****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

// Illegal operation counter
interrupt void ISR_ILLEGAL(void)
{
	static int16 illegal_count=0;

	illegal_count++;

}

// This function disable Watch Dog.
static void WDogDisable(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}

// This function initializes the PLLCR register.
//void InitPll(Uint16 val, Uint16 clkindiv)
static void PLLset(Uint16 val)
{
   volatile Uint16 iVol;

   // Make sure the PLL is not running in limp mode
   if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
   {
      // Missing external clock has been detected
      // Replace this line with a call to an appropriate
      // SystemShutdown(); function.
      asm("        ESTOP0");
   }

   // CLKINDIV MUST be 0 before PLLCR can be changed from
   // 0x0000. It is set to 0 by an external reset XRSn

   // Change the PLLCR
   if (SysCtrlRegs.PLLCR.bit.DIV != val)
   {

      EALLOW;
      // Before setting PLLCR turn off missing clock detect logic
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
      SysCtrlRegs.PLLCR.bit.DIV = val;
      EDIS;

      // Optional: Wait for PLL to lock.
      // During this time the CPU will switch to OSCCLK/2 until
      // the PLL is stable.  Once the PLL is stable the CPU will
      // switch to the new PLL value.
      //
      // This time-to-lock is monitored by a PLL lock counter.
      //
      // Code is not required to sit and wait for the PLL to lock.
      // However, if the code does anything that is timing critical,
      // and requires the correct clock be locked, then it is best to
      // wait until this switching has completed.

      // Wait for the PLL lock bit to be set.
      // The watchdog should be disabled before this loop, or fed within
      // the loop via ServiceDog().

	  // Uncomment to disable the watchdog
      WDogDisable();

      EALLOW;
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
//      SysCtrlRegs.PLLSTS.bit.CLKINDIV != clkindiv;

      EDIS;
    }

	//divide down SysClk by 2 to increase stability
	EALLOW;
    SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
    EDIS;
}

// This function initializes the PIE control registers to a known state.
//
static void PieCntlInit(void)
{
    // Disable Interrupts at the CPU level:
    DINT;

    // Disable the PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

	// Clear all PIEIER registers:
	PieCtrlRegs.PIEIER1.all = 0;
	PieCtrlRegs.PIEIER2.all = 0;
	PieCtrlRegs.PIEIER3.all = 0;
	PieCtrlRegs.PIEIER4.all = 0;
	PieCtrlRegs.PIEIER5.all = 0;
	PieCtrlRegs.PIEIER6.all = 0;
	PieCtrlRegs.PIEIER7.all = 0;
	PieCtrlRegs.PIEIER8.all = 0;
	PieCtrlRegs.PIEIER9.all = 0;
	PieCtrlRegs.PIEIER10.all = 0;
	PieCtrlRegs.PIEIER11.all = 0;
	PieCtrlRegs.PIEIER12.all = 0;

	// Clear all PIEIFR registers:
	PieCtrlRegs.PIEIFR1.all = 0;
	PieCtrlRegs.PIEIFR2.all = 0;
	PieCtrlRegs.PIEIFR3.all = 0;
	PieCtrlRegs.PIEIFR4.all = 0;
	PieCtrlRegs.PIEIFR5.all = 0;
	PieCtrlRegs.PIEIFR6.all = 0;
	PieCtrlRegs.PIEIFR7.all = 0;
	PieCtrlRegs.PIEIFR8.all = 0;
	PieCtrlRegs.PIEIFR9.all = 0;
	PieCtrlRegs.PIEIFR10.all = 0;
	PieCtrlRegs.PIEIFR11.all = 0;
	PieCtrlRegs.PIEIFR12.all = 0;
}

static void PieVectTableInit(void)
{
	int16	i;
	Uint32 *Source = (void *) &ISR_ILLEGAL;
	Uint32 *Dest = (void *) &PieVectTable;

	EALLOW;
	for(i=0; i < 128; i++)
		*Dest++ = *Source;
	EDIS;

	// Enable the PIE Vector Table
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
}

static void peripheral_InitDefault(void) {

	EALLOW; // below registers are "protected", allow access.

	// HIGH / LOW SPEED CLOCKS prescale register settings
//	SysCtrlRegs.HISPCP.all = 0x0001;	// HSPCLK = SYSCLKOUT / (2*HISPCP) = 75.0 MHz
	SysCtrlRegs.HISPCP.all = 0x0003;	// HSPCLK = SYSCLKOUT / (2*HISPCP) = 25.0 MHz
	SysCtrlRegs.LOSPCP.all = 0x0002;	// LSPCLK = SYSCLKOUT / (2*LOSPCP) = 37.5 MHz

	// XCLKOUT to SYSCLKOUT ratio.  By default XCLKOUT = 1/4 SYSCLKOUT
	   // XTIMCLK = SYSCLKOUT/2
//	   XintfRegs.XINTCNF2.bit.XTIMCLK = 1;
	   // XCLKOUT = XTIMCLK/2
//	   XintfRegs.XINTCNF2.bit.CLKMODE = 1;
	   // Enable XCLKOUT
//	   XintfRegs.XINTCNF2.bit.CLKOFF = 0;

//	ADC_cal();

	// PERIPHERAL CLOCK DISABLE
	//---------------------------------------------------
	// If you are not using a peripheral you may want to switch
	// the clock off to save power, i.e. set to =0
	//
	// Note: not all peripherals are available on all 280x derivates.
	// Refer to the datasheet for your particular device.


	   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;   // TBCLK
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0;    // ADC
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0;   // I2C
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR0.bit.SPIAENCLK=0;     // SPI-A
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK=0;     // SCI-A
	   SysCtrlRegs.PCLKCR0.bit.SCIBENCLK=0;     // SCI-B
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=0;    // eCAN-A
	   SysCtrlRegs.PCLKCR0.bit.ECANBENCLK=0;    // eCAN-B
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;  // eCAP1
	   SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 0;  // eCAP2
	   SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 0;  // eCAP3
	   SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 0;  // eCAP4
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 0;   // ePWM1
	   SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 0;   // ePWM2
	   SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 0;   // ePWM3
	   SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;   // ePWM4
	   SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;   // ePWM5
	   SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;   // ePWM6
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;   // eQEP1
	   SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;   // eQEP2
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 0; // timer0
       SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 0; // timer1
	   SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 0; // timer2
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 0; // DMA
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 0; // GPIO
	   //------------------------------------------------
	   SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 0; // XINTF


	EDIS;	// Disable register access
}

// This function initializes the Gpio to a known (default) state.
static void gpio_InitDefault(void) {

	EALLOW;

	// Each GPIO pin can be:
	// a) a GPIO input/output
	// b) peripheral function 1
	// c) peripheral function 2
	// d) peripheral function 3
	// By default, all are GPIO Inputs
	GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
	GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
	GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO39
	GpioCtrlRegs.GPBMUX2.all = 0x0000;     // GPIO functionality GPIO48-GPIO63
	GpioCtrlRegs.GPCMUX1.all = 0x0000;     // GPIO functionality GPIO64-GPIO79
	GpioCtrlRegs.GPCMUX2.all = 0x0000;     // GPIO functionality GPIO80-GPIO95

	GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are inputs
	GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO63 are inputs
	GpioCtrlRegs.GPCDIR.all = 0x0000;      // GPI064-GPIO95 are inputs

	// Each input can have different qualification
	// a) input synchronized to SYSCLKOUT
	// b) input qualified by a sampling window
	// c) input sent asynchronously (valid for peripheral inputs only)
	GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT
	GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
	GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO39 Synch to SYSCLKOUT
	GpioCtrlRegs.GPBQSEL2.all = 0x0000;    // GPIO48-GPIO63 Synch to SYSCLKOUT

	// Pull-ups can be enabled or disabled.
	GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
	GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO63
	GpioCtrlRegs.GPCPUD.all = 0x0000;      // Pullup's enabled GPIO64-GPIO79

	//GpioCtrlRegs.GPAPUD.all = 0xFFFF;    // Pullup's disabled GPIO0-GPIO31
	//GpioCtrlRegs.GPBPUD.all = 0xFFFF;    // Pullup's disabled GPIO32-GPIO34
	//GpioCtrlRegs.GPCPUD.all = 0xFFFF     // Pullup's disabled GPIO64-GPIO79

	EDIS;

}

/**************************** GLOBAL FUNCTIONS *******************************/

void DSP2833x_init(void) {

	WDogDisable(); 	// Disable the watchdog initially

	DINT;			// Global Disable all Interrupts
	IER = 0x0000;	// Disable CPU interrupts
	IFR = 0x0000;	// Clear all CPU interrupt flags

	PLLset(0xA);	// choose from options above based on the reference crystal

	PieCntlInit(); // Initializes interrupt controller to defaults
	PieVectTableInit(); // Initializes Vector Table to dafaults (Application ISR mapping done later).

	peripheral_InitDefault(); // Initializes peripheral

	gpio_InitDefault();	// Initializes gpios

}

void DeviceEnableInterrupts(void)
{

	PieCtrlRegs.PIEIER1.bit.INTx7=1;		// Enable TIMER 0 Interrupt
	PieCtrlRegs.PIEIER1.bit.INTx6=1;		// Enable ADC Interrupt
	PieCtrlRegs.PIEIER8.bit.INTx5=1;		// Enable SCI C RX Interrupt
	PieCtrlRegs.PIEIER9.bit.INTx3=1;		// Enable SCI B RX Interrupt
	PieCtrlRegs.PIEIER9.bit.INTx1=1;		// Enable SCI A RX Interrupt
	IER = M_INT1 | M_INT8| M_INT9;

	// Enables PIE to drive a pulse into the CPU
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1 | PIEACK_GROUP8 |PIEACK_GROUP9;	// Clear IRQ flag from group 1, 8 and 9.
	EINT;									// Enable INTM (Global Enable)

}

/// @}


