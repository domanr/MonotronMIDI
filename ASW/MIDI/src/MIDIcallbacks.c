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

#define CALIBRATION_CONSTANT	3
#define CC_ARP_ON_OFF			113
#define CC_ARP_PREV				114
#define CC_ARP_NEXT				115

uint8_t numberOfPressedKeys = 0;
uint8_t arpEnabled = false;

void mNoteOffCallback(Channel channel, DataByte data1, DataByte data2)
{
	//TEST: turn a LED off
//	LED1_Off();
	if(data1 != 127U) {
		numberOfPressedKeys--;
		if( numberOfPressedKeys == 0 ) {
			GateOff();
		}
	}
}

void mNoteOnCallback(Channel channel, DataByte data1, DataByte data2)
{
	//TEST: turn a LED on
//	LED1_On();
	if(data1 != 127U) {
		SetPitch(data1 * CALIBRATION_CONSTANT);
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
	if (data1 == CC_ARP_ON_OFF) {

	}
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

