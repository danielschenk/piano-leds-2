#include <Stream.h>

#include "ArduinoMidiInput.h"


ArduinoMidiInput::ArduinoMidiInput(Stream& serial)
    : serial(serial)
{
}

void ArduinoMidiInput::run()
{
    while(serial.available())
    {
        processMidiByte(static_cast<uint8_t>(serial.read()));
    }
}

unsigned int ArduinoMidiInput::getPortCount() const
{
    return 1;
}

void ArduinoMidiInput::openPort(int number)
{
    // Nothing to do.
}
