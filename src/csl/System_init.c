/*
 *  ==== DO NOT MODIFY THIS FILE - CHANGES WILL BE OVERWRITTEN ====
 *
 *  Generated from
 *      C:/ti/grace_1_10_00_17/packages/ti/mcu/msp430/csl/system/System_init.xdt
 */

#include <msp430.h>

/*
 *  ======== System_init ========
 *  Initialize MSP430 Status Register
 */
void System_init(void)
{
    /* 
     * IFG2, Interrupt Flag Register 2
     * 
     * ~UCB0TXIFG -- No interrupt pending
     * ~UCB0RXIFG -- No interrupt pending
     * ~UCA0TXIFG -- No interrupt pending
     * UCA0RXIFG -- Interrupt pending
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    IFG2 &= ~(UCA0RXIFG);

    /* 
     * IE2, Interrupt Enable Register 2
     * 
     * ~UCB0TXIE -- Interrupt disabled
     * ~UCB0RXIE -- Interrupt disabled
     * ~UCA0TXIE -- Interrupt disabled
     * UCA0RXIE -- Interrupt enabled
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    IE2 |= UCA0RXIE;

    /* 
     * SR, Status Register
     * 
     * ~SCG1 -- Disable System clock generator 1
     * ~SCG0 -- Disable System clock generator 0
     * ~OSCOFF -- Oscillator On
     * ~CPUOFF -- CPU On
     * GIE -- General interrupt enable
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    __bis_SR_register(GIE);
    
}
