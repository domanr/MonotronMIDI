/*
 * MIDI.c
 *
 *  Created on: May 16, 2014
 *      Author: Rajmi
 */

#include "../../../HAL/UART/include/SerialPort.h"
#include "../include/midi_Settings.h"
#include "../include/MIDI.h"

StatusByte  mRunningStatus_RX;
Channel     mInputChannel;
byte        mPendingMessage[3];
byte    mPendingMessageExpectedLenght;
byte    mPendingMessageIndex;
Message     mMessage;

int parse(void);
void handleNullVelocityNoteOnAsNoteOff(void);
Boolean inputFilter(Channel);
Boolean isChannelMessage(MidiType);
void resetInput(void);
void launchCallback(void);

extern void mNoteOffCallback(Channel channel, DataByte data1, DataByte data2);
extern void mNoteOnCallback(Channel channel, DataByte data1, DataByte data2);
extern void mClockCallback();
extern void mStartCallback();
extern void mContinueCallback();
extern void mStopCallback();
extern void mActiveSensingCallback();
extern void mControlChangeCallback(Channel channel, DataByte data1, DataByte data2);
extern void mPitchBendCallback(Channel channel, int data);
extern void mAfterTouchPolyCallback(Channel channel, DataByte data1, DataByte data2);
extern void mAfterTouchChannelCallback(Channel channel, DataByte data1);
extern void mProgramChangeCallback(Channel channel, DataByte data1);
extern void mSystemExclusiveCallback(DataByte*);
extern void mTimeCodeQuarterFrameCallback(DataByte data1);
extern void mSongPositionCallback(DataByte data);
extern void mSongSelectCallback(DataByte data1);
extern void mTuneRequestCallback();
extern void mSystemResetCallback();


#define		getType()			mMessage.type
#define		getChannel()		mMessage.channel
#define		getData1()			mMessage.data1
#define		getData2()			mMessage.data2
#define		getSysExArray()		mMessage.sysexArray
#define		check()				mMessage.valid
#define		getInputChannel()	mInputChannel

MidiType getTypeFromStatusByte(byte inStatus)
{
    if ((inStatus  < 0x80) ||
        (inStatus == 0xf4) ||
        (inStatus == 0xf5) ||
        (inStatus == 0xf9) ||
        (inStatus == 0xfD))
    {
        // Data bytes and undefined.
        return InvalidType;
    }
    if (inStatus < 0xf0)
    {
        // Channel message, remove channel nibble.
        return (MidiType)(inStatus & 0xf0);
    }

    return (MidiType)inStatus;
}

Channel getChannelFromStatusByte(byte inStatus)
{
    return (inStatus & 0x0f) + 1;
}

Boolean MidiRead(Channel inChannel)
{
    if (inChannel >= MIDI_CHANNEL_OFF)
        return false; // MIDI Input disabled.

    if (parse())
    {
        handleNullVelocityNoteOnAsNoteOff();
        if (inputFilter(inChannel))
        {
            launchCallback();
        	return true;
        }
    }

    return false;
}

int parse(void)
{
    //The buffer is checked if it's empty in the SerialPortRead

	/*if (GetAvailableBytes() == 0)
        // No data available.
        return false;
	*/

    // Parsing algorithm:
    // Get a byte from the serial buffer.
    // If there is no pending message to be recomposed, start a new one.
    //  - Find type and channel (if pertinent)
    //  - Look for other bytes in buffer, call parser recursively,
    //    until the message is assembled or the buffer is empty.
    // Else, add the extracted byte to the pending message, and check validity.
    // When the message is done, store it.

    const byte extracted = 0;
    if(!SerialPortRead((uint8_t*)&extracted))
    {
    	return false;
    }

    if (mPendingMessageIndex == 0)
    {
        // Start a new pending message
        mPendingMessage[0] = extracted;

        // Check for running status first
        if (isChannelMessage(getTypeFromStatusByte(mRunningStatus_RX)))
        {
            // Only these types allow Running Status

            // If the status byte is not received, prepend it
            // to the pending message
            if (extracted < 0x80)
            {
                mPendingMessage[0]   = mRunningStatus_RX;
                mPendingMessage[1]   = extracted;
                mPendingMessageIndex = 1;
            }
            // Else: well, we received another status byte,
            // so the running status does not apply here.
            // It will be updated upon completion of this message.
        }

        switch (getTypeFromStatusByte(mPendingMessage[0]))
        {
            // 1 byte messages
            case Start:
            case Continue:
            case Stop:
            case Clock:
            case ActiveSensing:
            case SystemReset:
            case TuneRequest:
                // Handle the message type directly here.
                mMessage.type    = getTypeFromStatusByte(mPendingMessage[0]);
                mMessage.channel = 0;
                mMessage.data1   = 0;
                mMessage.data2   = 0;
                mMessage.valid   = true;

                // \fix Running Status broken when receiving Clock messages.
                // Do not reset all input attributes, Running Status must remain unchanged.
                //resetInput();

                // We still need to reset these
                mPendingMessageIndex = 0;
                mPendingMessageExpectedLenght = 0;

                return true;
                break;

                // 2 bytes messages
            case ProgramChange:
            case AfterTouchChannel:
            case TimeCodeQuarterFrame:
            case SongSelect:
                mPendingMessageExpectedLenght = 2;
                break;

                // 3 bytes messages
            case NoteOn:
            case NoteOff:
            case ControlChange:
            case PitchBend:
            case AfterTouchPoly:
            case SongPosition:
                mPendingMessageExpectedLenght = 3;
                break;

            case SystemExclusive:
                // The message can be any lenght
                // between 3 and MIDI_SYSEX_ARRAY_SIZE bytes
                mPendingMessageExpectedLenght = MIDI_SYSEX_ARRAY_SIZE;
                mRunningStatus_RX = InvalidType;
                mMessage.sysexArray[0] = SystemExclusive;
                break;

            case InvalidType:
            default:
                // This is obviously wrong. Let's get the hell out'a here.
                resetInput();
                return false;
                break;
        }

        if (mPendingMessageIndex >= (mPendingMessageExpectedLenght - 1))
        {
            // Reception complete
            mMessage.type    = getTypeFromStatusByte(mPendingMessage[0]);
            mMessage.channel = getChannelFromStatusByte(mPendingMessage[0]);
            mMessage.data1   = mPendingMessage[1];

            // Save data2 only if applicable
            if (mPendingMessageExpectedLenght == 3)
                mMessage.data2 = mPendingMessage[2];
            else
                mMessage.data2 = 0;

            mPendingMessageIndex = 0;
            mPendingMessageExpectedLenght = 0;
            mMessage.valid = true;
            return true;
        }
        else
        {
            // Waiting for more data
            mPendingMessageIndex++;
        }

        #if USE_1BYTE_PARSING
        // Message is not complete.
        return false;
        #else
        // Call the parser recursively
        // to parse the rest of the message.
        return parse();
        #endif

    }
    else
    {
        // First, test if this is a status byte
        if (extracted >= 0x80)
        {
            // Reception of status bytes in the middle of an uncompleted message
            // are allowed only for interleaved Real Time message or EOX
            switch (extracted)
            {
                case Clock:
                case Start:
                case Continue:
                case Stop:
                case ActiveSensing:
                case SystemReset:

                    // Here we will have to extract the one-byte message,
                    // pass it to the structure for being read outside
                    // the MIDI class, and recompose the message it was
                    // interleaved into. Oh, and without killing the running status..
                    // This is done by leaving the pending message as is,
                    // it will be completed on next calls.

                    mMessage.type    = (MidiType)extracted;
                    mMessage.data1   = 0;
                    mMessage.data2   = 0;
                    mMessage.channel = 0;
                    mMessage.valid   = true;
                    return true;

                    break;

                    // End of Exclusive
                case 0xf7:
                    if (mMessage.sysexArray[0] == SystemExclusive)
                    {
                        // Store the last byte (EOX)
                        mMessage.sysexArray[mPendingMessageIndex++] = 0xf7;
                        mMessage.type = SystemExclusive;

                        // Get length
                        mMessage.data1   = mPendingMessageIndex & 0xff;
                        mMessage.data2   = mPendingMessageIndex >> 8;
                        mMessage.channel = 0;
                        mMessage.valid   = true;

                        resetInput();
                        return true;
                    }
                    else
                    {
                        // Well well well.. error.
                        resetInput();
                        return false;
                    }

                    break;
                default:
                    break;
            }
        }

        // Add extracted data byte to pending message
        if (mPendingMessage[0] == SystemExclusive)
            mMessage.sysexArray[mPendingMessageIndex] = extracted;
        else
            mPendingMessage[mPendingMessageIndex] = extracted;

        // Now we are going to check if we have reached the end of the message
        if (mPendingMessageIndex >= (mPendingMessageExpectedLenght - 1))
        {
            // "FML" case: fall down here with an overflown SysEx..
            // This means we received the last possible data byte that can fit
            // the buffer. If this happens, try increasing MIDI_SYSEX_ARRAY_SIZE.
            if (mPendingMessage[0] == SystemExclusive)
            {
                resetInput();
                return false;
            }

            mMessage.type = getTypeFromStatusByte(mPendingMessage[0]);

            if (isChannelMessage(mMessage.type))
                mMessage.channel = getChannelFromStatusByte(mPendingMessage[0]);
            else
                mMessage.channel = 0;

            mMessage.data1 = mPendingMessage[1];

            // Save data2 only if applicable
            if (mPendingMessageExpectedLenght == 3)
                mMessage.data2 = mPendingMessage[2];
            else
                mMessage.data2 = 0;

            // Reset local variables
            mPendingMessageIndex = 0;
            mPendingMessageExpectedLenght = 0;

            mMessage.valid = true;

            // Activate running status (if enabled for the received type)
            switch (mMessage.type)
            {
                case NoteOff:
                case NoteOn:
                case AfterTouchPoly:
                case ControlChange:
                case ProgramChange:
                case AfterTouchChannel:
                case PitchBend:
                    // Running status enabled: store it from received message
                    mRunningStatus_RX = mPendingMessage[0];
                    break;

                default:
                    // No running status
                    mRunningStatus_RX = InvalidType;
                    break;
            }
            return true;
        }
        else
        {
            // Then update the index of the pending message.
            mPendingMessageIndex++;

#if USE_1BYTE_PARSING
            // Message is not complete.
            return false;
#else
            // Call the parser recursively
            // to parse the rest of the message.
            return parse();
#endif
        }
    }
}

Boolean inputFilter(Channel inChannel)
{
    // This method handles recognition of channel
    // (to know if the message is destinated to the Arduino)

    if (mMessage.type == InvalidType)
        return false;

    // First, check if the received message is Channel
    if (mMessage.type >= NoteOff && mMessage.type <= PitchBend)
    {
        // Then we need to know if we listen to it
        if ((mMessage.channel == mInputChannel) ||
            (mInputChannel == MIDI_CHANNEL_OMNI))
        {
            return true;
        }
        else
        {
            // We don't listen to this channel
            return false;
        }
    }
    else
    {
        // System messages are always received
        return true;
    }
}

void handleNullVelocityNoteOnAsNoteOff(void)
{
    #if MIDI_HANDLE_NULL_VELOCITY_NOTE_ON_AS_NOTE_OFF
    if (getType() == NoteOn && getData2() == 0)
    {
        mMessage.type = NoteOff;
    }
    #endif
}

Boolean isChannelMessage(MidiType inType)
{
    return (inType == NoteOff           ||
            inType == NoteOn            ||
            inType == ControlChange     ||
            inType == AfterTouchPoly    ||
            inType == AfterTouchChannel ||
            inType == PitchBend         ||
            inType == ProgramChange);
}

void resetInput(void)
{
    mPendingMessageIndex = 0;
    mPendingMessageExpectedLenght = 0;
    mRunningStatus_RX = InvalidType;
}

void launchCallback(void)
{
    // The order is mixed to allow frequent messages to trigger their callback faster.
    switch (mMessage.type)
    {
            // Notes
        case NoteOff:               mNoteOffCallback(mMessage.channel, mMessage.data1, mMessage.data2);   break;
        case NoteOn:                mNoteOnCallback(mMessage.channel, mMessage.data1, mMessage.data2);    break;

            // Real-time messages
        case Clock:                 mClockCallback();           break;
        case Start:                 mStartCallback();           break;
        case Continue:              mContinueCallback();        break;
        case Stop:                  mStopCallback();            break;
        case ActiveSensing:         mActiveSensingCallback();   break;

            // Continuous controllers
        case ControlChange:         mControlChangeCallback(mMessage.channel, mMessage.data1, mMessage.data2);    break;
        case PitchBend:             mPitchBendCallback(mMessage.channel, (int)((mMessage.data1 & 0x7f) | ((mMessage.data2 & 0x7f) << 7)) + MIDI_PITCHBEND_MIN); break; // TODO: check this
        case AfterTouchPoly:        mAfterTouchPolyCallback(mMessage.channel, mMessage.data1, mMessage.data2);    break;
        case AfterTouchChannel:     mAfterTouchChannelCallback(mMessage.channel, mMessage.data1);    break;

        case ProgramChange:         mProgramChangeCallback(mMessage.channel, mMessage.data1);    break;
        case SystemExclusive:       mSystemExclusiveCallback(mMessage.sysexArray);    break;

            // Occasional messages
        case TimeCodeQuarterFrame:  mTimeCodeQuarterFrameCallback(mMessage.data1);    break;
        case SongPosition:          mSongPositionCallback((mMessage.data1 & 0x7f) | ((mMessage.data2 & 0x7f) << 7));    break;
        case SongSelect:            mSongSelectCallback(mMessage.data1);    break;
        case TuneRequest:           mTuneRequestCallback();    break;

        case SystemReset:           mSystemResetCallback();    break;
        case InvalidType:
        default:
            break;
    }
}
