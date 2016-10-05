/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

/*
 * ======== Grace related includes ========
 */
#include <ti/mcu/msp430/csl/CSL.h>

/*
 * ======== Application includes ========
 */
#include "ASW/MIDI/include/MIDI.h"

/*
 * ======== Defines ========
 */

#define	MIDI_INPUT_CHANNEL	(Channel)10U

/*
 *  ======== main ========
 */
int main(int argc, char *argv[])
{
    CSL_init();                     // Activate Grace-generated configuration
    
    // >>>>> Fill-in user code here <<<<<
    
    while(1)
    {
    	MidiRead(MIDI_INPUT_CHANNEL);
    }
}
