/*
 * MIDIcallbacks.c
 *
 *  Created on: 2014.08.14.
 *      Author: domanr
 */

#include "../include/MIDI.h"
#include "../../../Drivers/LED/include/LED.h"
#include "../../Pitch/include/Pitch.h"
#include "../../Gate/include/Gate.h"


#define UPPER_LIMIT(x, y) (((x) > (y)) ? (y) : (x))
#define LOWER_LIMIT(x, y) (((x) < (y)) ? (y) : (x))

#define CALIBRATION_CONSTANT	2
#define ARP_MAX 3

/* Lookup table parameters */
#define LOWEST_NOTE		24u
#define HIGHEST_NOTE	101u

#define NUMBER_OF_NOTES (HIGHEST_NOTE - LOWEST_NOTE + 1u)

uint8_t lookupTable[NUMBER_OF_NOTES] =
{
		0,
		3,
		7,
		10,
		13,
		17,
		20,
		23,
		26,
		30,
		33,
		36,
		40,
		43,
		46,
		50,
		53,
		56,
		59,
		63,
		66,
		69,
		73,
		76,
		79,
		83,
		86,
		89,
		92,
		96,
		99,
		102,
		106,
		109,
		112,
		116,
		119,
		122,
		125,
		129,
		132,
		135,
		139,
		142,
		145,
		149,
		152,
		155,
		158,
		162,
		165,
		168,
		172,
		175,
		178,
		182,
		185,
		188,
		191,
		195,
		198,
		201,
		205,
		208,
		211,
		215,
		218,
		221,
		224,
		228,
		231,
		234,
		238,
		241,
		244,
		248,
		251,
		254
};

uint8_t arpeggioDiff[ARP_MAX+1] = {0, 3, 5, 8};
uint8_t arpIndex = 0;

uint8_t numberOfPressedKeys = 0;
uint8_t arpEnabled = false;
uint8_t currentNote = 0;

void mNoteOffCallback(Channel channel, DataByte data1, DataByte data2)
{
	if(data1 != 127U) {
		numberOfPressedKeys--;
		if( numberOfPressedKeys == 0 ) {
			GateOff();
		}
	}
}

void mNoteOnCallback(Channel channel, DataByte data1, DataByte data2)
{
	if(data1 != 127U) {
		arpIndex = 1;
		if(data1 < LOWEST_NOTE) {
			data1 = LOWEST_NOTE;
		} else if(data1 > HIGHEST_NOTE) {
			data1 = HIGHEST_NOTE;
		} else {
			/* Keep the value */
		}
		currentNote = data1;
		SetPitch( lookupTable[data1 - LOWEST_NOTE] );
		numberOfPressedKeys++;
		GateOn();
	}
}

void mClockCallback()
{

}

void mStartCallback()
{

}

void mContinueCallback()
{

}

void mStopCallback()
{

}

void mActiveSensingCallback()
{

}

void mControlChangeCallback(Channel channel, DataByte data1, DataByte data2)
{

}

void mPitchBendCallback(Channel channel, int data)
{

}

void mAfterTouchPolyCallback(Channel channel, DataByte data1, DataByte data2)
{

}

void mAfterTouchChannelCallback(Channel channel, DataByte data1)
{

}

void mProgramChangeCallback(Channel channel, DataByte data1)
{
	if( MIDI_INPUT_CHANNEL == channel ) {
		if( data1 != 0u ) {
			arpEnabled = true;
		}
		else {
			arpEnabled = false;
		}
	}
}

void mSystemExclusiveCallback(DataByte* sysexArray)
{

}

void mTimeCodeQuarterFrameCallback(DataByte data1)
{

}

void mSongPositionCallback(DataByte data)
{

}

void mSongSelectCallback(DataByte data1)
{

}

void mTuneRequestCallback()
{

}

void mSystemResetCallback()
{

}

void BPM_TimerHandler(void)
{
	uint8_t nextNote = 0;

	if(arpEnabled) {
		nextNote = UPPER_LIMIT( currentNote + arpeggioDiff[arpIndex], HIGHEST_NOTE );
		SetPitch( lookupTable[nextNote - LOWEST_NOTE] );
			arpIndex++;
			if(arpIndex > ARP_MAX) {
				arpIndex = 0;
			}
	}
}
