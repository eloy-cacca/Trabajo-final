/*
 * byte_string_operations.c
 *
 *
 * Created: 11/09/2015 01:07:57 p.m.
 *  Author: JF
 */
#include "byte_string_operations.h"

// =======================================================================================
// EXPORTED GLOBAL FUNCTIONS
// =======================================================================================
void copy_content_form_16bits_to_8bits(uint16_t *source_16bit, uint8_t *target_8bit, uint16_t length)
{
    uint16_t i = 0;

    for (i = 0; i < length; i++) {
    	if (!(i % 2)) // is odd
    		target_8bit[i] = 0x00FF & (source_16bit[i>>1]>>0);
    	else
    		target_8bit[i] = 0x00FF & (source_16bit[i>>1]>>8);
    }
}

void copy_content_form_8bits_to_16bits(uint8_t *source_8bits, uint16_t *target_16bits, uint16_t length)
{
    uint16_t i = 0;

    for (i = 0; i < length; i++) {
    	if (!(i % 2)) // is odd
    		target_16bits[i>>1]  =  (0x00FF & source_8bits[i]);
    	else
    		target_16bits[i>>1] |= ((0x00FF & source_8bits[i])<<8);
    }
}

void copy_content(uint8_t *source, uint8_t *target, uint16_t length)
{
    uint16_t i = 0;

    for (i = 0; i < length; i++)
        target[i] = source[i];
}

// length is in bytes, return flag: mismatch
uint16_t compare_content(volatile uint8_t *in1, uint8_t *in2, uint16_t length)
{
    static uint16_t i = 0;

    for (i = 0; i < length; i++) {
        if (in1[i] != in2[i]) {
            // if content unmatched
            return 1;
        }
    }
    // if content is matched
    return 0;
}

uint16_t calculate_checksum(uint8_t input[], uint32_t length)
{
    uint16_t checksum = 0;

    calculate_checksum_ptr(input, length, &checksum);

    return checksum;
}

// returns 16-bit checksum. If done for a packet with checksum appended, should return 0
// length is in bytes!
// be careful this function is not reentrant
void calculate_checksum_ptr(uint8_t input[], uint32_t length, uint16_t *result)
{
    static int16_t CRCPoly = 0xA001;
    static uint32_t i = 0;
    static uint8_t j = 0;
    static uint8_t data = 0;
    static uint8_t result_bit = 0;
    static uint8_t data_bit = 0;

    // for every byte
    for (i = 0; i < length; i++) {
        data = input[i];
        // for every bit
        for (j = 0; j < 8; j++) {
 
            result_bit = *result & 0x1;    // get LSbit
            *result = *result >> 1;        // shift down
 
            data_bit = data & 0x1;        // get LSbit
            data = data >> 1;            // shift down
 
            if (result_bit != data_bit) {
                *result = *result ^ CRCPoly;
            }
        }
    }
}

//Takes one Ascii byte and converts it to hex
//e.g. : converts 'A' (which is 0x41) to 0x0A
uint8_t ascii2hex_byte(uint8_t c)
{
    static uint8_t out;

    //check correct input range
    out = 0x00;

    if ((c & 0xF0) == 0x30) {
        out = c & 0x0F;
    }

    if ((c & 0xF0) == 0x40) {
        out = (c & 0x0F) + 9;
    }

    if ((c & 0xF0) == 0x60) {
        out = (c & 0x0F) + 9;
    }

    return out;
}

uint8_t calculate_string_length(uint8_t * str , uint8_t length_max) {
	
	uint8_t temp_length = 0;
	
	for (;;) {
		if ( (str[temp_length] != 0) & (temp_length < length_max) )
			temp_length++;
		else
			return temp_length;
	}	
	
}