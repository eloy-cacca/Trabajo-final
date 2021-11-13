// $Header
/// @file			meter_ctrl.c
/// @brief  		Meter control application.
/// @details		It define the state of supply module and raspberry pi.
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		30 October 2017
/// @details
/// @copyright
///

/// @addtogroup apps
/// @{

/**************************** INCLUDES ***************************************/

#include "meter_ctrl.h"
#include "system.h"
#include "rtc.h"
#include "control.h"
#include "unit_config.h"
#include "../utils/iqmath.h"


/**************************** MACROS *****************************************/

#define VIN_Q11_THRESHOLD      TOFIX( 10.0 , 11 )

#define COUNT_MS_1SECOND       ((uint32_t)1000)
#define COUNT_MS_5SECOND       ((uint32_t)5000)
#define COUNT_MS_10SECOND      ((uint32_t)10000)
#define COUNT_MS_15SECOND      ((uint32_t)15000)
#define COUNT_MS_30SECOND      ((uint32_t)30000)
#define COUNT_MS_1MINUTE       ((uint32_t)60000)
#define COUNT_MS_5MINUTE       ((uint32_t)300000)

/**************************** ENUMS, STRUCTS *********************************/

enum meter_ctrl_state_enum meter_ctrl_state;

uint32_t count_ms = 0;

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

/**************************** GLOBAL FUNCTIONS *******************************/

/// @brief Initialized meter control.
void meter_ctrl_init(void) {

	// By default, battery is disable
	meter_ctrl_battery_disable();
	meter_ctrl_rpi_turn_off();
	meter_ctrl_rpi_supply_disable();

	// State machine initial value
	meter_ctrl_state = METER_CTRL_STATE_INIT;

	// counter
	count_ms = 0;

}

/// @brief Update meter control depending on input voltage.
/// @details Input voltage is measured and state of meter is defined.
///
/// @param[in] vin_Q11 Pointer to input voltage in Q11.
///
void meter_ctrl_update(int32_t * vin_Q11) {

	// always increment count when RTC set a RTC_FLAG_MSECOND flags in the actual period.
	if (rtc_get_flags() && RTC_FLAG_MSECOND)
		count_ms++;

#if 0

	meter_ctrl_battery_enable();

#else

	switch(meter_ctrl_state) {

		case METER_CTRL_STATE_INIT: // check input voltage for a specific time

			// set flags
			meter_ctrl_battery_disable();
			meter_ctrl_rpi_turn_off();
			meter_ctrl_rpi_supply_disable();

			// Start again counter if input voltage is < threshold
			if ( *vin_Q11 < VIN_Q11_THRESHOLD ) {
				count_ms = 0;
			}
			// if count reach specific value, go to next state
			else if (count_ms > COUNT_MS_5SECOND ) {
				meter_ctrl_battery_enable();
				meter_ctrl_rpi_turn_on();
				meter_ctrl_rpi_supply_enable();
				meter_ctrl_state = METER_CTRL_STATE_IDLE;
			}

			break;

		case METER_CTRL_STATE_IDLE: // all device enable.

			// Go to following state if input voltage is < threshold
			if ( *vin_Q11 < VIN_Q11_THRESHOLD) {
				count_ms = 0; // start again
				meter_ctrl_state = METER_CTRL_STATE_CONFIRM_LOW_VIN;
			}

			break;

		case METER_CTRL_STATE_CONFIRM_LOW_VIN: // check low voltage for specific time

			// if voltage is ok return to previous state
			if ( *vin_Q11 > VIN_Q11_THRESHOLD) {
				meter_ctrl_state = METER_CTRL_STATE_IDLE;
			}
			// if voltage is still low, turn off RPI and go to next state
			else if (count_ms > COUNT_MS_1SECOND ) {
				meter_ctrl_battery_enable();
				meter_ctrl_rpi_turn_off();
				meter_ctrl_rpi_supply_enable();
				count_ms = 0; // start again
				meter_ctrl_state = METER_CTRL_STATE_WAIT_RPI_TURNOFF;
			}

			break;

		case METER_CTRL_STATE_WAIT_RPI_TURNOFF: // wait RPI turn off

			// if specific time was reach, disable RPI supply and go to next state
			if (count_ms > COUNT_MS_15SECOND ) {
				meter_ctrl_battery_enable();
				meter_ctrl_rpi_turn_off();
				meter_ctrl_rpi_supply_disable();
				count_ms = 0; // start again
				meter_ctrl_state = METER_CTRL_STATE_LOW_VIN;
			}

			break;

		case METER_CTRL_STATE_LOW_VIN: // wait interruption period set by standard

			// if voltage is ok return to previous state
			if ( *vin_Q11 > VIN_Q11_THRESHOLD) {
				count_ms = 0;
				meter_ctrl_state = METER_CTRL_STATE_INIT;
			}
			// if specific time was reach, go to next state
			else if (count_ms > COUNT_MS_1MINUTE ) {		//TODO: it should be 3 minutes
				meter_ctrl_battery_enable();
				meter_ctrl_rpi_turn_off();
				meter_ctrl_rpi_supply_disable();
				count_ms = 0; // start again
				meter_ctrl_state = METER_CTRL_STATE_SHUTDOWN_1;
			}

			break;

		case METER_CTRL_STATE_SHUTDOWN_1: // wait specific time until shutdown meter

			// if specific time was reach, shutdown meter
			if ( (count_ms > COUNT_MS_1SECOND ) &&													// TODO: it should be 1 second.
#if SETTINGS_ARM_ENABLE_SDCARD_COMM_FLAG_SAVE
					(0 == (comm_flags & (!(SETTINGS_COMM_FLAG_TEST01 | SETTINGS_COMM_FLAG_TEST02)))) ){
#else
					(0 == (system_flags.comm_dsp)) ){
#endif
				meter_ctrl_battery_enable();
				meter_ctrl_rpi_turn_off();
				meter_ctrl_rpi_supply_disable();
				count_ms = 0; // start again
				meter_ctrl_state = METER_CTRL_STATE_SHUTDOWN_2;
			}

			break;

		case METER_CTRL_STATE_SHUTDOWN_2: // wait specific time (used for any ARM task)

			// if specific time was reach, shutdown meter
			if (count_ms > COUNT_MS_1SECOND ) {
				meter_ctrl_battery_disable();
				meter_ctrl_rpi_turn_off();
				meter_ctrl_rpi_supply_disable();
				count_ms = 0; // start again
				meter_ctrl_state = METER_CTRL_STATE_SHUTDOWN_3;
			}

			break;


		default: // METER_CTRL_STATE_SHUTDOWN_3

			meter_ctrl_battery_disable();
			meter_ctrl_rpi_turn_off();
			meter_ctrl_rpi_supply_disable();

			if (count_ms > COUNT_MS_1SECOND ) {
				meter_ctrl_battery_disable();
				meter_ctrl_rpi_turn_off();
				meter_ctrl_rpi_supply_disable();
				count_ms = 0; // start again
				meter_ctrl_state = METER_CTRL_STATE_INIT; // this point should never be reached
			}

			break;

	}

   	// update trend flag
	if ( meter_ctrl_state > METER_CTRL_STATE_IDLE )
		*ctrl_trend_flag_temp_ptr() |= SETTINGS_TREND_FLAGS_LOW_VIN;

#endif

}

/// @brief Request actual state.
enum meter_ctrl_state_enum * meter_ctrl_get_state(void) {
	return &meter_ctrl_state;
}

/// @}

