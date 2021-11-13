// $Header
/// @file			system.c
/// @brief  		DSP module.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		29 February 2016
/// @details
/// @copyright
///

/// @addtogroup modules
/// @{

/**************************** INCLUDES ***************************************/

#include "system.h"

/**************************** MACROS *****************************************/

#define LSPCLK 		(37.5e6)
#define baud  		SETTINGS_COMM_DSP_BAUDRATE
#define BRR 		((int16)( ((LSPCLK / baud) / 8.0) - 1.0))

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

static void flash_init(void) {

#if defined(_FLASH)
	// Copy time critical code and Flash setup code to RAM
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the linker files.
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
	InitFlash(); // Call the flash wrapper init function
#endif

}

static void gpio_init(void) {

	EALLOW;

	// Enable GPIO input clock
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;

	// Test Point
	{
		// TEST POINT 1
		GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;	// 0=GPIO,  1=I2C-SDA,  2=SYNCI,  3=ADCSOCA
		GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;		// 1=OUTput,  0=INput
		GpioDataRegs.GPBSET.bit.GPIO32 = 1;		// Set High initially

		// TEST POINT 2
		GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;	// 0=GPIO,  1=I2C-SCL,  2=SYNCO,  3=ADCSOCB
		GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;		// 1=OUTput,  0=INput
		GpioDataRegs.GPBSET.bit.GPIO33 = 1;		// Set High initially

		// TEST POINT 3
		GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;	// 0=GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;		// 1=OUTput,  0=INput
		GpioDataRegs.GPASET.bit.GPIO21 = 0;		// Set High initially

		// TEST POINT 4
		GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;		// 0=GPIO,  1=I2C-SDA,  2=SYNCI,  3=ADCSOCA
		GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;		// 1=OUTput,  0=INput
		GpioDataRegs.GPASET.bit.GPIO3 = 1;		// Set High initially

	}

	// Meter control
/*	{
		// Battery enable
		GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;	// 0=GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;		// 1=OUTput,  0=INput
		GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;    	// Disable pull-up on GPIO2 (EPWM2A)
		GpioDataRegs.GPASET.bit.GPIO22 = 0;		// Set High initially

		// RPI turn off
		GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;	    // 0=GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;		// 1=OUTput,  0=INput
		GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    	// Disable pull-up on GPIO2 (EPWM2A)
		GpioDataRegs.GPASET.bit.GPIO0 = 0;		// Set High initially

		// RPI Supply
		GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;	    // 0=GPIO
		GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;		// 1=OUTput,  0=INput
		GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    	// Disable pull-up on GPIO2 (EPWM2A)
		GpioDataRegs.GPASET.bit.GPIO1 = 0;		// Set High initially

	}*/

	// DACs (PWM channels)
	{

		GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   	// Configure GPIO2 as EPWM2A
		GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;    	// Disable pull-up on GPIO2 (EPWM2A)

		GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   	// Configure GPIO4 as EPWM3A
		GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;    	// Disable pull-up on GPIO4 (EPWM3A)

	}

	// control debugging
/*	{

		GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   	// Configure GPIO0 as EPWM1A
		GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    	// Disable pull-up on GPIO0 (EPWM1A)

	}*/

	// leds
	{
		// LED RED 1
		GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;	// 0=GPIO,  1=I2C-SCL,  2=SYNCO,  3=ADCSOCB
		GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;		// 1=OUTput,  0=INput
		GpioDataRegs.GPASET.bit.GPIO31 = 1;		// Set High initially

		// LED RED 2
		GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;	// 0=GPIO,  1=I2C-SCL,  2=SYNCO,  3=ADCSOCB
		GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;		// 1=OUTput,  0=INput
		GpioDataRegs.GPBSET.bit.GPIO34 = 1;		// Set High initially
	}

	// communication
	{
		// SCI-A
		GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
		GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)
		GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
		GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation
		GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)

		// SCI-B
//		GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;    // Enable pull-up for GPIO22 (SCITXDC)
//		GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;	   // Enable pull-up for GPIO23 (SCIRXDC)
//		GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;   // Configure GPIO22 for SCIRXDB operation
//		GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO23 for SCITXDB operation
//		GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (SCIRXDC)

		// SCI-C
//		GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
//		GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)
//		GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 for SCIRXDC operation
//		GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 for SCITXDC operation
//		GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)
	}

	EDIS;


}

static void sci_a(void) {

	EALLOW;

	// Enable SYSCLKOUT to SCIA
	SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;

	// sci-a configuration
 	SciaRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
                                    // No parity,8 char bits,
                                    // async mode, idle-line protocol
 	SciaRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                    // Disable RX ERR, SLEEP, TXWAKE
 	SciaRegs.SCICTL2.all = 0x0000;  // Disable TX interrupt
 									// Disable Rx interrupt

 	SciaRegs.SCIHBAUD    = (BRR>>8) & 0xFF;
    SciaRegs.SCILBAUD    = (BRR>>0) & 0xFF;

    // sci-a FIFO configuration
    SciaRegs.SCIFFTX.all=0xE040; // SCI FIFO no reset state, SCI FIFO enable, re-enable SCI FIFO from reset
    							 // interrupt has not occurred, clear interrupt flag, disable interrupt
 //   SciaRegs.SCIFFRX.all=0x204f; // Receive FIFO has not overflow, Receive FIFO reset,
    							 // clear interrupt flag, disable interrupt, Received FIFO interrupt level = b11111

    SciaRegs.SCIFFRX.all=0x2061;

    SciaRegs.SCIFFCT.all=0x0;

	// ISR used in this project are re-mapped to PIE Vector
    PieVectTable.SCIRXINTA = &sci_a_rx_irq;

    // enable sci-a
    SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset


	EDIS;

}

/*static void sci_c(void) {

	EALLOW;

	// Enable SYSCLKOUT to SCIA
	SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;

	// sci-a configuration
 	ScicRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
 	ScicRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE

 	ScicRegs.SCICTL2.all = 0x0000;  // Disable TX interrupt
 									// Disable Rx interrupt

 	ScicRegs.SCIHBAUD    = (BRR>>8) & 0xFF;
    ScicRegs.SCILBAUD    = (BRR>>0) & 0xFF;

    // sci-a FIFO configuration
    ScicRegs.SCIFFTX.all=0xE040; // SCI FIFO no reset state, SCI FIFO enable, re-enable SCI FIFO from reset
    							 // interrupt has not occurred, clear interrupt flag, disable interrupt
    ScicRegs.SCIFFRX.all=0x2061; // Receive FIFO has not overflow, Receive FIFO reset,
    							 // clear interrupt flag, enable interrupt, Received FIFO interrupt level = 1


    ScicRegs.SCIFFCT.all=0x0;

	// ISR used in this project are re-mapped to PIE Vector
    PieVectTable.SCIRXINTC = &sci_c_rx_irq;

    // enable sci-c
    ScicRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset

	EDIS;

}*/

/*static void sci_b(void) {

	EALLOW;

	// Enable SYSCLKOUT to SCIA
	SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;

	// sci-a configuration
 	ScibRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
 	ScibRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE

 	ScibRegs.SCICTL2.all = 0x0000;  // Disable TX interrupt
 									// Disable Rx interrupt

 	ScibRegs.SCIHBAUD    = (BRR>>8) & 0xFF;
    ScibRegs.SCILBAUD    = (BRR>>0) & 0xFF;

    // sci-a FIFO configuration
    ScibRegs.SCIFFTX.all=0xE040; // SCI FIFO no reset state, SCI FIFO enable, re-enable SCI FIFO from reset
    							 // interrupt has not occurred, clear interrupt flag, disable interrupt
    ScibRegs.SCIFFRX.all=0x2061; // Receive FIFO has not overflow, Receive FIFO reset,
    							 // clear interrupt flag, enable interrupt, Received FIFO interrupt level = 1


    ScibRegs.SCIFFCT.all=0x0;

	// ISR used in this project are re-mapped to PIE Vector
    PieVectTable.SCIRXINTB = &sci_b_rx_irq;

    // enable sci-c
    ScibRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset

	EDIS;

}*/

/// @brief Initialize CPU TIMER 1.
/// @details CPU TIMER 1 - to measure the time utilization of control service routine
static void cputimers1_init(void) {

	EALLOW;

	// Enable SYSCLKOUT to CPU-Timer 1
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1;

	EDIS;

	// TIMER1 Configuration
	CpuTimer1Regs.TCR.bit.TSS = 1;     	// stop timer
	CpuTimer1Regs.PRD.all = 0xFFFFFFFF; // set initial TIMER1 period
	CpuTimer1Regs.TPR.all = 0;         	// set prescaler (lower 16)
	CpuTimer1Regs.TPRH.all  = 0;       	// set prescaler (upper 16)
	CpuTimer1Regs.TCR.bit.TRB = 1;     	// 1 = reload timer
	CpuTimer1Regs.TCR.bit.TIE = 0;     	// 1 = Enable Timer Interrupt
	CpuTimer1Regs.TCR.bit.FREE = 1;    	// run free
}

/// @brief Initialize PWM to be used as DACs.
static void pwm_dacs(void) {

	EALLOW;  // This is needed to write to EALLOW protected register

	// EPWM2A
	{
		// Enable SYSCLKOUT to PWM
		SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;
		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

		// Setup TBCLK
		EPwm2Regs.TBCTL.bit.CTRMODE 	= TB_FREEZE; 	// Start turn off
		EPwm2Regs.TBPRD 				= 0xFFFF;       // Set timer period
		EPwm2Regs.TBCTL.bit.PHSEN 		= TB_DISABLE;   // Disable phase loading
		EPwm2Regs.TBPHS.half.TBPHS 		= 0x0000;       // Phase is 0
		EPwm2Regs.TBCTR 				= 0x0000;		// Clear counter
		EPwm2Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;   	// Clock ratio to SYSCLKOUT
		EPwm2Regs.TBCTL.bit.CLKDIV 		= TB_DIV1;

		// Setup shadow register load on ZERO
		EPwm2Regs.CMPCTL.bit.SHDWAMODE  = CC_SHADOW;
		EPwm2Regs.CMPCTL.bit.LOADAMODE  = CC_CTR_ZERO;

		// Set Compare values
		EPwm2Regs.CMPA.half.CMPA 		= 7500; // Set compare A value (aprox 50us for a 150MHz clock)
		// Set actions
		EPwm2Regs.AQCTLA.bit.ZRO        = AQ_SET;   // Set PWM2A on Zero
		EPwm2Regs.AQCTLA.bit.CAU        = AQ_CLEAR; // Clear PWM2A on event A, up count

		// Start
		EPwm2Regs.TBCTL.bit.CTRMODE     = TB_COUNT_UP;

		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	}

	// EPWM3A
	{
		// Enable SYSCLKOUT to PWM
		SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;
		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

		// Setup TBCLK
		EPwm3Regs.TBCTL.bit.CTRMODE 	= TB_FREEZE; 	// Start turn off
		EPwm3Regs.TBPRD 				= 0xFFFF;       // Set timer period
		EPwm3Regs.TBCTL.bit.PHSEN 		= TB_DISABLE;   // Disable phase loading
		EPwm3Regs.TBPHS.half.TBPHS 		= 0x0000;       // Phase is 0
		EPwm3Regs.TBCTR 				= 0x0000;		// Clear counter
		EPwm3Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;   	// Clock ratio to SYSCLKOUT
		EPwm3Regs.TBCTL.bit.CLKDIV 		= TB_DIV1;

		// Setup shadow register load on ZERO
		EPwm3Regs.CMPCTL.bit.SHDWAMODE  = CC_SHADOW;
		EPwm3Regs.CMPCTL.bit.LOADAMODE  = CC_CTR_ZERO;

		// Set Compare values
		EPwm3Regs.CMPA.half.CMPA 		= 7500; // Set compare A value (aprox 50us for a 150MHz clock)
		// Set actions
		EPwm3Regs.AQCTLA.bit.ZRO        = AQ_SET;   // Set PWM2A on Zero
		EPwm3Regs.AQCTLA.bit.CAU        = AQ_CLEAR; // Clear PWM1A on event A, up count

		// Start
		EPwm3Regs.TBCTL.bit.CTRMODE     = TB_COUNT_UP;

		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	}

	EDIS;	// This is needed to disable write to EALLOW protected registers

}

#if SETTINGS_ISR_TYPE

/// @brief Initialize CPU TIMER 0.
/// @details CPU TIMER 0 - to implement the control interrupt
static void cputimers0_init(void) {

    EALLOW;

    // Enable SYSCLKOUT to CPU-Timer 0
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;

	// ISR used in this project are re-mapped to PIE Vector
    PieVectTable.TINT0 = &cputimer0_irq;

    EDIS;

	// TIMER0 Configuration
	CpuTimer0Regs.TCR.bit.TSS = 1;     				// stop timer
	CpuTimer0Regs.PRD.all = 0xFFFF;					// set initial TIMER0 period
	CpuTimer0Regs.TPR.all = 0;         				// set prescaler (lower 16)
	CpuTimer0Regs.TPRH.all  = 0;       				// set prescaler (upper 16)
	CpuTimer0Regs.TCR.bit.TRB = 1;     				// 1 = reload timer
	CpuTimer0Regs.TCR.bit.TIE = 1;     				// 1 = Enable Timer Interrupt
	CpuTimer0Regs.TCR.bit.FREE = 1;    				// run free

}

#else

static void adc_pwm_init(void) {

	EALLOW;  // This is needed to write to EALLOW protected register

	// ADC
	{

		// Enable SYSCLKOUT to ADC
		SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;

		// Power up bandgap/reference/ADC circuits
		AdcRegs.ADCTRL3.all = 0xE0;

		// ADC clock prescaler -> ADCCLK = HISPCP/2 = 75MHz/2 =  37.5MHz
		AdcRegs.ADCTRL1.bit.CPS = 1;

		// Acquisition window size
		AdcRegs.ADCTRL1.bit.ACQ_PS = 1; // 2 ADCCLK

		// ISR used in this project are re-mapped to PIE Vector
		PieVectTable.ADCINT = &adc_isr;

		// Configuration
		AdcRegs.ADCTRL1.bit.SEQ_CASC 		= 0x1;	// Cascaded mode. SEQ1 and SEQ2 operate as a single 16-state sequencer.
		AdcRegs.ADCTRL3.bit.SMODE_SEL 		= 0x1; 	// Setup simultaneous sampling mode
		AdcRegs.ADCMAXCONV.all 				= 0x4;  // Setup 4 conv's on SEQ1 and 1 conv on SEQ2
		AdcRegs.ADCCHSELSEQ1.bit.CONV00 	= 0x7; 	// Setup ADCINA7 and ADCINB7 as 1st and 2nd SEQ1 conversions.
		AdcRegs.ADCCHSELSEQ1.bit.CONV01 	= 0x6; 	// Setup ADCINA6 and ADCINB6 as 3rd and 4th SEQ1 conversions.
		AdcRegs.ADCCHSELSEQ1.bit.CONV02 	= 0x5; 	// Setup ADCINA5 and ADCINB5 as 5th and 6th SEQ1 conversions.
		AdcRegs.ADCCHSELSEQ1.bit.CONV03 	= 0x4; 	// Setup ADCINA4 and ADCINB4 as 7th and 8th SEQ1 conversions.
		//AdcRegs.ADCCHSELSEQ2.bit.CONV04 	= 0x3; 	// Setup ADCINA3 and ADCINB3 as 1th and 2th SEQ2 conversions.
		AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1	= 0x1;	// Enable SOCA from ePWM to start SEQ1
		AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1	= 0x1; 	// Enable SEQ1 interrupt (every EOS)

	}

	// PWM
	{

		// Enable SYSCLKOUT to PWM
		SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;
		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

		// Setup TBCLK
		EPwm1Regs.TBCTL.bit.CTRMODE 	= TB_FREEZE; 	// Start turn off
		EPwm1Regs.TBPRD 				= 0xFFFF;       // Set timer period
		EPwm1Regs.TBCTL.bit.PHSEN 		= TB_DISABLE;   // Disable phase loading
		EPwm1Regs.TBPHS.half.TBPHS 		= 0x0000;       // Phase is 0
		EPwm1Regs.TBCTR 				= 0x0000;		// Clear counter
		EPwm1Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;   	// Clock ratio to SYSCLKOUT
		EPwm1Regs.TBCTL.bit.CLKDIV 		= TB_DIV1;

		// Setup shadow register load on ZERO
		EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
		EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;

		// Set Compare values
		EPwm1Regs.CMPA.half.CMPA = 7500; // Set compare A value (aprox 50us for a 150MHz clock)
		// Set actions
		EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
		EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A, up count

		// Event trigger selection
		EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group
		EPwm1Regs.ETSEL.bit.SOCASEL = 4;       // Select SOC from from CPMA on upcount
		EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event

		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;

	}

	EDIS;	// This is needed to disable write to EALLOW protected registers

}

#endif

static void dsp_ready(void) {

	Uint16 repeat;

	DELAY_US(1000000);	// Wait for ARM be ready (1 second)
	// Enviar FW Version
	// Enviar RTC
	// Enviar DSP configuration and settings

	// turn on and off leds
	for (repeat=0;repeat<3;repeat++) {

		dsp_red_led_control_on();
		dsp_red_led_comm_on();
		DELAY_US(250000);

		dsp_red_led_control_off();
		dsp_red_led_comm_off();
		DELAY_US(250000);

	}

}

/**************************** GLOBAL FUNCTIONS *******************************/

void system_init() {


	DSP2833x_init();   // Default Initialization
	flash_init();
	gpio_init();
	sci_a();
//	sci_b();
//	sci_c();
	pwm_dacs();
#if SETTINGS_ISR_TYPE
	cputimers0_init();
#else
	adc_pwm_init();
#endif
	cputimers1_init();

}

void system_ready(void) {

	dsp_ready();

}

/// @}

