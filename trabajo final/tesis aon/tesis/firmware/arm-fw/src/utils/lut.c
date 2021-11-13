/** $Header
 * @file			lut.c
 * @brief 			Look up table.
 * @author 			Ignacio Carugati
 * @version   		1.0
 * @date      		29 February 2016
 * @details			Sine and Cosine functions are implemented. Sine table is
 *					stored in FLASH.
 * @copyright
 *
 * @addtogroup utilities
 * @{
 */

/**************************** INCLUDES ***************************************/
#include "lut.h"
#include "utils/byte_string_operations.h"

/**************************** MACROS *****************************************/
#define LUT_SIN_128_N_075	((int32_t)(LUT_128_N * 0.75))
#define LUT_SIN_128_N_025	((int32_t)(LUT_128_N * 0.25))


/**************************** LOCAL VARIABLES ********************************/
const int32_t table_sin_128_Q20[LUT_128_N] = {0,51451,102778,153858,204567,254783,304385,353254,401272,448324,494295,539075,582557,624635,665209,704180,741455,776943,810560,842224,871859,899393,924761,947901,968757,987280,1003424,1017151,1028427,1037226,1043526,1047312,1048576,1047312,1043526,1037226,1028427,1017151,1003424,987280,968757,947901,924761,899393,871859,842224,810560,776943,741455,704180,665209,624635,582557,539075,494295,448324,401272,353254,304385,254783,204567,153858,102778,51451,0,-51452,-102779,-153859,-204568,-254784,-304386,-353255,-401273,-448325,-494296,-539076,-582558,-624636,-665210,-704181,-741456,-776944,-810561,-842225,-871860,-899394,-924762,-947902,-968758,-987281,-1003425,-1017152,-1028428,-1037227,-1043527,-1047313,-1048576,-1047313,-1043527,-1037227,-1028428,-1017152,-1003425,-987281,-968758,-947902,-924762,-899394,-871860,-842225,-810561,-776944,-741456,-704181,-665210,-624636,-582558,-539076,-494296,-448325,-401273,-353255,-304386,-254784,-204568,-153859,-102779,-51452};
int32_t table_custom_128_Q20[LUT_128_N] = {0};

/**************************** GLOBAL FUNCTIONS *******************************/

/**
 * @brief Sine function (table 128 values)
 * @param[in] phase Input phase in ticks [0-127]. It is obtained by affecting the phase in rad with a gain equal of 128/(2*pi).
 */
int32_t lut_sin_128_Q20(uint8_t phase)
{
	return table_sin_128_Q20[phase];
}

/**
 * @brief Cosine function (table 128 values)
 * @param[in] phase Input phase in ticks [0-127]. It is obtained by affecting the phase in rad with a gain equal of 128/(2*pi).
 */
int32_t lut_cos_128_Q20(uint8_t phase)
{
	return table_sin_128_Q20[ phase > (LUT_SIN_128_N_075-1) ? phase-LUT_SIN_128_N_075 : phase+LUT_SIN_128_N_025 ];
}

/**
 * @brief Custom function (table 128 values)
 * @param[in] phase Input phase in ticks [0-127].
 */
int32_t lut_custom_128(uint8_t phase)
{
	return table_custom_128_Q20[phase];
}

/**
 * @brief Set custom LUT table
 * @param[in] phase Input phase in ticks [0-127].
 */
void set_custom_lut(int32_t * new_table)
{
	copy_content((uint8_t *) new_table, (uint8_t *)&table_custom_128_Q20[0], LUT_128_N * sizeof(int32_t));
}
