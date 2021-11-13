/**
 * @file           loop.c
 * @brief          loop function.
 * @details
 * @author         Agustin Aon
 * @version        0.1
 * @date           14 April 2021
 * @details
 * @copyright
 *
 * @addtogroup apps_dsp
 * @{
 */


/**************************** INCLUDES ***************************************/
#include "loop.h"


/**************************** MACROS *****************************************/
#define BUF_LENGTH        (136)


/**************************** LOCAL VARIABLES ********************************/

/**
 * La ecuacion en diferencias sacada de matlab es asi:
 *    in_cst[0].in[0] + in_cst[1].in[1] + ... = out_cst[0].out[0] + out_cst[1].out[1] + ...
 *
 *  Donde dice 1 en realidad es -1 y asi, claramente.
 */
static int32_t in_cst[BUF_LENGTH] = {0};        // Q24
static int32_t out_cst[BUF_LENGTH] = {0};       // Q24

// Buffer circulares
static int32_t in_buf[BUF_LENGTH] = {0};
static uint16_t in_buf_ix = 0;
static int32_t out_buf[BUF_LENGTH] = {0};
static uint16_t out_buf_ix = 0;


/**************************** GLOBAL FUNCTIONS *******************************/

/** @brief Loop variable init. */
void loop_init()
{
    in_cst[0] = 3.721075565017595E+2;
    in_cst[1] = -6.381639241112242E+1;
    in_cst[2] = -1.981531602457314E+2;
    in_cst[3] = -6.50346761230699E+1;
    in_cst[4] = -7.16994562663807;
    in_cst[5] = -9.983555425917709E-2;
    in_cst[6] = 1.581609836518337E-2;
    in_cst[125] = 2.097152E+3;
    in_cst[126] = 1.595204006106066E+3;
    in_cst[127] = -3.559888512574864E+3;
    in_cst[128] = -2.849327853967273E+3;
    in_cst[129] = 1.141498278095201E+3;
    in_cst[130] = 1.230930777290284E+3;
    in_cst[131] = 3.311737722021439E+2;
    in_cst[132] = 3.354318046855984E+1;
    in_cst[133] = 3.14303705210275E-1;
    in_cst[134] = -9.924084949182407E-2;
    in_cst[135] = 1.027620453737239E-3;


    out_cst[0] = 3.721075565017594E+5;
    out_cst[1] = -6.381639241112242E+4;
    out_cst[2] = -1.981531602457314E+5;
    out_cst[3] = -6.50346761230699E+4;
    out_cst[4] = -7.169945626638069E+3;
    out_cst[5] = -9.983555425917709E+1;
    out_cst[6] = 1.581609836518337E+1;
    out_cst[127] = -9.302688912543986E+4;
    out_cst[128] = -1.700996801480991E+5;
    out_cst[129] = -1.15804028584458E+4;
    out_cst[130] = 1.312893472564138E+5;
    out_cst[131] = 8.384811452962732E+4;
    out_cst[132] = 1.986860073265131E+4;
    out_cst[133] = 1.83845015919781E+3;
    out_cst[134] = 1.705083938220259E+1;
    out_cst[135] = -3.954024591295843;
}

/** @brief Loop update output. */
void loop_update(int32_t * data_in, int32_t * data_out)
{
    // Accumulate input in buffer
    in_buf[in_buf_ix] = *data_in;

    // Control loop
    uint16_t i;
    uint16_t temp_in_ix = in_buf_ix;
    uint16_t temp_out_ix = out_buf_ix;
    int64_t temp = 0;

    for (i = 0; i < BUF_LENGTH; i++)
    {
        if (in_cst[0] != 0)
            temp += in_cst[i] * in_buf[temp_in_ix];

        if (i != 0 && out_cst[i] != 0)
            temp -= out_cst[i] * out_buf[temp_out_ix];

        temp_in_ix++;
        temp_out_ix++;
        if (temp_in_ix >= BUF_LENGTH)
            temp_in_ix -= BUF_LENGTH;
        if (temp_out_ix >= BUF_LENGTH)
            temp_out_ix -= BUF_LENGTH;
    }

    temp /= out_cst[0];
    out_buf[out_buf_ix] = (int32_t) temp;        // cambiar Q? ahora esta en Q0

    // Return result
    *data_out = out_buf[out_buf_ix];

    // Increase indexes
    in_buf_ix++;
    out_buf_ix++;

    if (in_buf_ix >= BUF_LENGTH)
        in_buf_ix = 0;
    if (out_buf_ix >= BUF_LENGTH)
        out_buf_ix = 0;
}


/** @}*/
