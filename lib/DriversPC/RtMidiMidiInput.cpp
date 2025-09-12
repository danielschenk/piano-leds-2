#include "RtMidiMidiInput.h"

#include <RtMidi.h>

#include <cassert>
#include <cstdio>

RtMidiMidiInput::RtMidiMidiInput() : rtMidiIn(new RtMidiIn())
{
    assert(rtMidiIn != nullptr);
    rtMidiIn->setCallback(&RtMidiCommonCallback, (void *)this);
}

RtMidiMidiInput::~RtMidiMidiInput()
{
    delete rtMidiIn;
}

unsigned int RtMidiMidiInput::getPortCount() const
{
    return rtMidiIn->getPortCount();
}

void RtMidiMidiInput::openPort(int number)
{
    rtMidiIn->openPort(number);
}

void RtMidiMidiInput::RtMidiCommonCallback(double deltatime, std::vector<unsigned char> *message,
                                           void *userData)
{
    // userData tells us the instance to call the specific callback on
    RtMidiMidiInput *midiInput = static_cast<RtMidiMidiInput *>(userData);
    if (midiInput != nullptr)
    {
        midiInput->RtMidiCallback(deltatime, message);
    }
}

void RtMidiMidiInput::RtMidiCallback(double deltatime, std::vector<unsigned char> *message)
{
    for (auto byte : *message)
    {
        processMidiByte(byte);
    }
}
