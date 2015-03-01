/*
 * SerialPort_Cfg.c
 *
 *  Created on: Mar 1, 2015
 *      Author: Rajmi
 */

#include "../include/SerialPort.h"
#include "../include/SerialPort_Cfg.h"

uint8_t SerialPortWrite(uint8_t data) {
	while (!(IFG2&UCA0TXIFG));
	UCA0TXBUF = data;

	return (uint8_t) 1;
}
