// $Header
/// @file			supply_monitor.c
/// @brief  		Supply monitor apps.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		20 Febrary 2017
/// @details
/// @copyright
///

/// @addtogroup apps
/// @{

/**************************** INCLUDES ***************************************/

#include "supply_monitor.h"
#include "system.h"

/**************************** MACROS *****************************************/

#define PMC_MASK_STATUS0	(0xFFFFFFFC)	/** Bit mask for peripheral clocks (PCER0) */
#define PMC_MASK_STATUS1	(0xFFFFFFFF)	/** Bit mask for peripheral clocks (PCER1) */
#define PIO_MASK_PORT		(0xFFFFFFFF)

/**************************** ENUMS, STRUCTS *********************************/

/**************************** FUNCTION PROTOTYPES ****************************/

/**************************** LOCAL VARIABLES ********************************/

/**************************** GLOBAL VARIABLES *******************************/

/**************************** LOCAL FUNCTIONS ********************************/

void backup_mode_prepare(void)
{
	// Disable interrupts
	system_disable_interrupt();

	// Configure all PIOs as input, disable pull ups and pull downs
	gpio_configure_group( PIOA , PIO_MASK_PORT , (PIO_INPUT | PIO_DEFAULT));
	gpio_configure_group( PIOB , PIO_MASK_PORT , (PIO_INPUT | PIO_DEFAULT));
	pio_pull_up(PIOA, PIO_MASK_PORT , false );
	pio_pull_up(PIOB, PIO_MASK_PORT , false );
	pio_pull_down(PIOA, PIO_MASK_PORT , false );
	pio_pull_down(PIOB, PIO_MASK_PORT , false );
			
	// Configure PIO for wake up
	pio_pull_down(PIOA, PIO_PA0 , true );
	
	// Disable all peripheral clocks 
	pmc_disable_all_periph_clk();
	
	// Disable brown out detector and supply monitor
	supc_disable_brownout_detector( SUPC );	
	supc_set_monitor_sampling_period( SUPC , SUPC_SMMR_SMSMPL_SMD );
	
	// Disable USB device clock and programmable clocks
	PMC->PMC_SCDR = PMC_SCDR_UDP | PMC_SCDR_PCK0 | PMC_SCDR_PCK1 | PMC_SCDR_PCK2;
	
}

void enter_backup_mode(void)
{	
	// Enable the GPIO for wake-up
	supc_set_wakeup_inputs( SUPC , SUPC_WUIR_WKUPEN0_ENABLE , SUPC_WUIR_WKUPT0_HIGH );
//	supc_set_wakeup_mode( SUPC , SUPC_WUMR_SMEN );
	
	// Switch MCK to slow clock 
	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);
	
	// Disable unused clock to save power - Disable fast RC oscillator and PLL
	pmc_osc_disable_fastrc();
	pmc_osc_disable_xtal( 0 );
	pmc_disable_pllack();
	
	// Enable back up mode
	pmc_enable_backupmode();
	
	/* Note: The core will reset when exiting from backup mode. */
}
