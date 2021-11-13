// $Header
/// @file			DSP2833x_drivers_flash.h
/// @brief  		Flash drivers.
/// @details
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		11 February 2015
/// @details
/// @copyright
///

#ifndef __DSP28335_DRIVERS_FLASH_H__
#define __DSP28335_DRIVERS_FLASH_H__

/**************************** INCLUDES ***************************************/

#include "DSP2833x_library.h"

/**************************** MACROS *****************************************/

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

// Used for running functions in RAM
extern Uint16 RamfuncsLoadStart, RamfuncsLoadEnd, RamfuncsRunStart;

/**************************** FUNCTIONS **************************************/

/// @cond

void InitFlash(void);
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);

/// @endcond

#endif // __DSP28335_DRIVERS_FLASH_H__

