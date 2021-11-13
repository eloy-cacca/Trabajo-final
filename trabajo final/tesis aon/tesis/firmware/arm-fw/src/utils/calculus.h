// $Header
/// @file           calculus.h
/// @brief          Single calculus definition.
/// @author 		Ignacio Carugati
/// @version   		0.1
/// @date      		15 October 2014
/// @details
/// @copyright
///

#ifndef __CALCULUS_H__
#define __CALCULUS_H__

/**************************** INCLUDES ***************************************/

/**************************** MACROS *****************************************/

#define SIGNUM(x)     ( (x) > 0 ? 1 : -1 )
#define MAX(x,y)      ( (x) > (y) ? (x) : (y) )
#define MIN(x,y)      ( (x) < (y) ? (x) : (y) )
#define ABS(x)        ( (x) > (0) ? (x) : (-x) )
#define SQUARE(x)     ( (x) * (x) )

/**************************** ENUMS, STRUCTS *********************************/

/**************************** TYPEDEF ****************************************/

/**************************** VARIABLES **************************************/

/**************************** FUNCTIONS **************************************/

/// @cond

/// @endcond

#endif // __CALCULUS_H__

