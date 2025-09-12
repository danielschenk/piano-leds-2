#include "MidiMessageLogger.hpp"

#include "Logging.hpp"

#define LOGGING_COMPONENT "MidiMessageLogger"

MidiMessageLogger::MidiMessageLogger(IMidiInput& midiInput) : midiInput(midiInput)
{
    midiInput.subscribe(*this);
}

MidiMessageLogger::~MidiMessageLogger()
{
    midiInput.unsubscribe(*this);
}

void MidiMessageLogger::onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on)
{
    LOG_DEBUG_PARAMS("%3s chan %2u pitch %3u vel %3u", on ? "ON" : "OFF", channel, pitch, velocity);
}

void MidiMessageLogger::onControlChange(uint8_t channel,
                                        IMidiInterface::TControllerNumber controller, uint8_t value)
{
    LOG_DEBUG_PARAMS("CON chan %2u controller %3u val %3u", channel, controller, value);
}

void MidiMessageLogger::onProgramChange(uint8_t channel, uint8_t program)
{
    LOG_DEBUG_PARAMS("PRG chan %2u num %2u", channel, program);
}

void MidiMessageLogger::onChannelPressureChange(uint8_t channel, uint8_t value)
{
    LOG_DEBUG_PARAMS("CHP chan %2u val %2u", channel, value);
}

void MidiMessageLogger::onPitchBendChange(uint8_t channel, uint16_t value)
{
    LOG_DEBUG_PARAMS(" PB chan %2u val %5u", channel, value);
}
