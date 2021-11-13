/*
 * byte_string_operations.h
 *
 * Created: 11/09/2015 01:50:44 p.m.
 *  Author: JF
 */

#ifndef BYTE_STRING_OPERATIONS_H_
#define BYTE_STRING_OPERATIONS_H_

/**************************** INCLUDES ***************************************/
#include "settings.h"
//#include <stdint.h>

/**************************** MACROS *****************************************/
#define CLEAR_FLAG( reg , flag )	reg &= ~flag
#define SET_FLAG( reg , flag )	    reg |= flag
#define FLAG_IS_SET( reg , flag )   ( (flag) == (reg & (flag)) )

/**************************** ENUMS, STRUCTS *********************************/
typedef enum {
	FALSE = 0,
	TRUE
} bool_t;

/**************************** FUNCTIONS **************************************/
void copy_content_form_16bits_to_8bits(uint16_t *source_16bit, uint8_t *target_8bit, uint16_t length);
void copy_content_form_8bits_to_16bits(uint8_t *source_8bits, uint16_t *target_16bits, uint16_t length);

void copy_content(uint8_t *source, uint8_t *target, uint16_t length);
uint16_t compare_content(volatile uint8_t *in1, uint8_t *in2, uint16_t length);
uint16_t calculate_checksum(uint8_t input[], uint32_t length);
void calculate_checksum_ptr(uint8_t input[], uint32_t length, uint16_t *result);
uint8_t ascii2hex_byte(uint8_t c);

uint8_t calculate_string_length(uint8_t * str , uint8_t length_max);

#endif /* BYTE_STRING_OPERATIONS_H_ */
