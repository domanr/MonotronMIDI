/*
 * PWM.h
 *
 *  Created on: Oct 21, 2014
 *      Author: Rajmi
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>
#include "PWM_Cfg.h"

#define SetDutyCycle(DutyCycle)		( DUTY_CYCLE_REG = (uint8_t) DutyCycle )

#endif /* PWM_H_ */
