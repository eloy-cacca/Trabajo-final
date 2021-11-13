// $Header
/// @file		main.c
/// @brief  		main function.
/// @details
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		27 November 2014
/// @details
/// @copyright
///

/// @addtogroup main
/// @{

/*************************** INCLUDES ****************************************/


#include "settings.h"

#include "apps/system.h"
#include "apps/control.h"
#include "apps/unit_config.h"
#include "apps/rtc.h"
#include "apps/meter_ctrl.h"

#include "apps_comm/comm.h"

/*************************** MACROS ******************************************/

/*************************** ENUMS, STRUCTS **********************************/

/*************************** FUNCTION PROTOTYPES *****************************/

/*************************** LOCAL VARIABLES *********************************/

/*************************** GLOBAL VARIABLES ********************************/

/*************************** LOCAL FUNCTIONS *********************************/

/*************************** GLOBAL FUNCTIONS ********************************/

///@brief 	Main function.
void main()
{

	system_init();						// DSP Initialization
	unit_config_and_calibration_init();	// Unit configuration initialization
	control_init();						// control module initialization
	rtc_init();							// Real Time clock initialization
//	meter_ctrl_init();                  // Meter control initialization
	comm_init();						// Communication initialization
	system_ready();						// Device Ready
	control_start();					// Control module start

	for(;;) {

		comm_background();
		control_background();

	}

}

#if SETTINGS_ISR_TYPE
/// @brief CpuTimer0 Interrupt service routine.
/// @details This function is executed when cputimer0 interrupt is detected. It
///          set and clear the test point used for measure the time utilization
///          in a oscilloscope and run the control module which all the processing
///          tasks.
interrupt void cputimer0_irq(void) {

	test_point_04_set();		// update test point for utilization time
	control_update();			// control module update
	test_point_04_clr();		// update test point for utilization time

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Clear IRQ flag from group 1 (CPUTIMER 0)

}
#else
/// @brief ADC Interrupt service routine.
/// @details This function is executed when all ADC channels reach a EOS (end of
///			 conversion). PWM generate the SOC (Start of conversion).
interrupt void  adc_isr(void)
{

	test_point_04_set();		// update test point for utilization time
	control_update();			// control module update
	test_point_04_clr();		// update test point for utilization time

	// Reinitialize for next ADC sequence
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE

}
#endif

/// @brief SCI-A RX FIFO Interrupt service routine.
/// @details This function is executed when a byte is receive in SCI RX A. Bytes
///          are stored in FIFO buffer and if at less one is received, interrupt
///          service routine is executed. In this function, data is stored in
///          corresponding register.
interrupt void sci_a_rx_irq(void) {

	EINT; 	// allow execution of others interrupt (mainly TIMER0) while this service routine is executed
	comm_sci_a_rx();
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9; // Clear IRQ flag from group 9 (SCI-A)

}

/// @brief SCI-B RX FIFO Interrupt service routine.
/// @details This function is executed when a byte is receive in SCI RX B. Bytes
///          are stored in FIFO buffer and if at less one is received, interrupt
///          service routine is executed. In this function, data is stored in
///          corresponding register.
/*interrupt void sci_b_rx_irq(void) {

	EINT; 	// allow execution of others interrupt (mainly TIMER0) while this service routine is executed
	comm_sci_b_rx();
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9; // Clear IRQ flag from group 9 (SCI-A)

}*/

/// @brief SCI-C RX FIFO Interrupt service routine.
/// @details This function is executed when a byte is receive in SCI RX C. Bytes
///          are stored in FIFO buffer and if at less one is received, interrupt
///          service routine is executed. In this function, data is stored in
///          corresponding register.
/*interrupt void sci_c_rx_irq(void) {

	EINT; 	// allow execution of others interrupt (mainly control) while this service routine is executed
	comm_sci_c_rx();
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8; // Clear IRQ flag from group 8 (SCI-C)

}*/

/// @}

