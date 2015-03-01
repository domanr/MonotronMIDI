/*
 * LED.h
 *
 *  Created on: Mar 1, 2015
 *      Author: Rajmi
 */

#ifndef LED_H_
#define LED_H_

#include "../../../HAL/Dio/include/Dio_Interfaces.h"
#include "LED_Cfg.h"

#define LED1_On()		IOSet( LED1_PORT, LED1_BIT )
#define LED1_Off()		IOClear( LED1_PORT, LED1_BIT )
#define LED1_Toggle()	IOToggle( LED1_PORT, LED1_BIT )

#define LED2_On()		IOSet( LED2_PORT, LED2_BIT )
#define LED2_Off()		IOClear( LED2_PORT, LED2_BIT )
#define LED2_Toggle()	IOToggle( LED2_PORT, LED2_BIT )


#endif /* LED_H_ */
