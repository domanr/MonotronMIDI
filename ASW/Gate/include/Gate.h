/*
 * Gate.h
 *
 *  Created on: Oct 21, 2014
 *      Author: Rajmi
 */

#ifndef GATE_H_
#define GATE_H_

#include "../../../HAL/Dio/include/Dio_interfaces.h"
#include "Gate_Cfg.h"

#define GateOn()	IOSet( GATE_PORT, GATE_PIN )
#define GateOff()	IOClear( GATE_PORT, GATE_PIN )

#endif /* GATE_H_ */
