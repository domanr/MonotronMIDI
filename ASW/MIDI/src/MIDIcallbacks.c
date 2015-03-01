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

void mNoteOffCallback(Channel channel, DataByte data1, DataByte data2)
{
	//TEST: turn a LED off
	LED1_Off();

	GateOff();
}

void mNoteOnCallback(Channel channel, DataByte data1, DataByte data2)
{
	//TEST: turn a LED on
	LED1_On();
	SetPitch(data1 << 1);
	GateOn();
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

