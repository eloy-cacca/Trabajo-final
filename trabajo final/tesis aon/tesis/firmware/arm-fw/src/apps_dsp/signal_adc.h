/** $Header
 * @file           signal_adc.h
 * @brief          adc function.
 * @details
 * @author         Agustin Aon
 * @version        0.1
 * @date           21 July 2020
 * @details
 * @copyright
 */

#ifndef SIGNAL_ADC_H_
#define SIGNAL_ADC_H_


/**************************** INCLUDES ***************************************/
#include "settings.h"


/**************************** FUNCTIONS **************************************/
void signal_adc_init(void);
void signal_adc_start_conversion(void);
void signal_adc_process_data(void);
void signal_adc_update(int32_t *signals);


#endif /* SIGNAL_ADC_H_ */