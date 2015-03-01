/*
 * SerialPort.c
 *
 *  Created on: May 16, 2014
 *      Author: Rajmi
 */

#include "stdint.h"
#include "../include/SerialPort.h"
#include "../include/SerialPort_Cfg.h"

#define	BUFFER_SIZE	32

uint8_t SerialBuffer[BUFFER_SIZE];
uint8_t	ReadPosition = 0;
uint8_t WritePosition = 0;


CbReadReturnType SerialPortRead(uint8_t* data) {
	if(ReadPosition == WritePosition)
	{
		return eErrorBufferEmpty;
	}

	*data = SerialBuffer[ReadPosition];
	ReadPosition = (ReadPosition + 1) % BUFFER_SIZE;
	return eReadOK;
}

uint8_t GetBufferLength() {
	return((WritePosition - ReadPosition) & (BUFFER_SIZE-1));
}

void UARTRxISR() {
	if(GetBufferLength() == BUFFER_SIZE-1)
	{
//			return eErrorBufferFull;
			return;
	}
	SerialBuffer[WritePosition] = UART_InputBuffer;
	WritePosition = (WritePosition + 1) % BUFFER_SIZE;
	return;
}
