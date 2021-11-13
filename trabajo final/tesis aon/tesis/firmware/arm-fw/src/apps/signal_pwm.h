/**
 * @file          pwm.h
 * @brief         pwm file.
 * @details
 * @author        Agustin Aon
 * @version       0.1
 * @date          11 April 2021
 * @details
 * @copyright
 */

#ifndef SIGNAL_PWM_H_
#define SIGNAL_PWM_H_


/**************************** INCLUDES ***************************************/
#include "settings.h"


/**************************** FUNCTIONS **************************************/
void signal_pwm_init(void);
void signal_pwm_enable(bool b_enable);


#endif // SIGNAL_PWM_H_
