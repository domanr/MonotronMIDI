/*****************************************************************
 *  File:			Dio_Interfaces.h
 *
 *  Created on: 	17.12.2014
 *  Author: 		Rajmund Doman
 *  Description: 	Interfaces for the digital I/O ports
 *
 *****************************************************************/

#ifndef DIO_INTERFACES_H_
#define DIO_INTERFACES_H_

/*
 * ======== Standard MSP430 includes ========
 */
#include	<msp430.h>
#include	<stdint.h>


/*
 * ======== Macros for digital outputs ========
 */
#define	IOSet(port,bit)					ExpandTokenIOSet(port,bit)
#define ExpandTokenIOSet(port,bit)		(port##OUT |= bit)

#define	IOClear(port,bit)				ExpandTokenIOClear(port,bit)
#define ExpandTokenIOClear(port,bit)	(port##OUT &= ~bit)

#define IOToggle(port,bit)				ExpandTokenIOToggle(port,bit)
#define ExpandTokenIOToggle(port,bit)	(port##OUT ^= bit)


/*
 * ======== Macros for digital inputs ========
 */
#define IOReadPin(port,bit)						ExpandTokenIOReadPin(port,bit)
#define ExpandTokenIOReadPin(port,bit)			( (port##IN & bit) != 0 )
#define IOReadPort(port)						ExpandTokenIOReadPort(port)
#define ExpandTokenIOReadPort(port)				( (uint8_t) port##IN )

/*
 * ======== Macros for interrupt flags ========
 */
#define IOPortSetInterruptFlag(port,bit)				ExpandTokenIOPortSetInterruptFlag(port,bit)
#define ExpandTokenIOPortSetInterruptFlag(port,bit)		( port##IFG |= bit )

#define IOPortClearInterruptFlag(port,bit)				ExpandTokenIOPortClearInterruptFlag(port,bit)
#define ExpandTokenIOPortClearInterruptFlag(port,bit)	( port##IFG &= ~bit )

#define IOPortInterruptFlagIsSet(port,bit)				ExpandTokenIOPortInterruptFlagIsSet(port,bit)
#define ExpandTokenIOPortInterruptFlagIsSet(port,bit)	( port##IFG & bit )

#endif /* DIO_INTERFACES_H_ */
