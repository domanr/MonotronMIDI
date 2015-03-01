/*
 *  ==== DO NOT MODIFY THIS FILE - CHANGES WILL BE OVERWRITTEN ====
 *
 *  Generated from
 *      C:/ti/grace_1_10_00_17/packages/ti/mcu/msp430/csl/communication/USCI_A0_init.xdt
 */

#include <msp430.h>

/*
 *  ======== USCI_A0_init ========
 *  Initialize Universal Serial Communication Interface A0 UART 2xx
 */
void USCI_A0_init(void)
{
    /* Disable USCI */
    UCA0CTL1 |= UCSWRST;

    /* 
     * Control Register 1
     * 
     * UCSSEL_2 -- SMCLK
     * ~UCRXEIE -- Erroneous characters rejected and UCAxRXIFG is not set
     * ~UCBRKIE -- Received break characters do not set UCAxRXIFG
     * ~UCDORM -- Not dormant. All received characters will set UCAxRXIFG
     * ~UCTXADDR -- Next frame transmitted is data
     * ~UCTXBRK -- Next frame transmitted is not a break
     * UCSWRST -- Enabled. USCI logic held in reset state
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    UCA0CTL1 = UCSSEL_2 + UCSWRST;
    
    /* Baud rate control register 1 */
    UCA0BR1 = 1;
    
    /* Enable USCI */
    UCA0CTL1 &= ~UCSWRST;
}
