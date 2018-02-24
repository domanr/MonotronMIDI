/*
 *  ======== CSL_init.c ========
 *  DO NOT MODIFY THIS FILE - CHANGES WILL BE OVERWRITTEN
 */
 
/* external peripheral initialization functions */
extern void GPIO_init(void);
extern void BCSplus_init(void);
extern void USCI_A0_init(void);
extern void Timer0_A3_init(void);
extern void System_init(void);
extern void Timer1_A3_init(void);
extern void WDTplus_init(void);

#include <msp430.h>

/*
 *  ======== CSL_init =========
 *  Initialize all configured CSL peripherals
 */
void CSL_init(void)
{
    /* Stop watchdog timer from timing out during initial start-up. */
    WDTCTL = WDTPW + WDTHOLD;

    /* initialize Config for the MSP430 GPIO */
    GPIO_init();

    /* initialize Config for the MSP430 2xx family clock systems (BCS) */
    BCSplus_init();

    /* initialize Config for the MSP430 USCI_A0 */
    USCI_A0_init();

    /* initialize Config for the MSP430 A3 Timer0 */
    Timer0_A3_init();

    /* initialize Config for the MSP430 System Registers */
    System_init();

    /* initialize Config for the MSP430 A3 Timer0 */
    Timer1_A3_init();

    /* initialize Config for the MSP430 WDT+ */
    WDTplus_init();

}

/*
 *  ======== Interrupt Function Definitions ========
 */
/* --COPYRIGHT--,EPL
 *  Copyright (c) 2010 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 * 
 *  Contributors:
 *      Texas Instruments - initial implementation
 * 
 * --/COPYRIGHT--*/

/* Interrupt Function Prototypes */
extern void UARTRxISR(void);

extern void BPM_TimerHandler(void);






/*
 *  ======== USCI A0/B0 RX Interrupt Handler Generation ========
 */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK(void)
{
	/* USCI_A0 Receive Interrupt Handler */
	UARTRxISR();

	/* No change in operating mode on exit */
}

/*
 *  ======== Timer1_A3 Interrupt Service Routine ======== 
 */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR_HOOK(void)
{

	/* Capture Compare Register 0 ISR Hook Function Name */
	BPM_TimerHandler();

	/* No change in operating mode on exit */
}


