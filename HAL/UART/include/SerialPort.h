/*
 * SerialPort.h
 *
 *  Created on: May 4, 2014
 *      Author: Rajmi
 */

#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include "stdint.h"

typedef enum
{
	eErrorBufferFull	= 0,
	eWriteOK			= 1
}CbWriteReturnType;

typedef enum
{
	eErrorBufferEmpty	= 0,
	eReadOK				= 1
}CbReadReturnType;

extern void UARTRxISR(void);
extern uint8_t SerialPortWrite(uint8_t);
extern CbReadReturnType SerialPortRead(uint8_t*);
extern uint8_t GetBufferLength(void);

#define GetAvailableBytes()	GetBufferLength()


#endif /* SERIALPORT_H_ */
