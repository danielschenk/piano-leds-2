#include "MidiInput.hpp"

void MidiInput::Observer::onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) {}
void MidiInput::Observer::onControlChange(uint8_t channel, ControllerNumber controller,
                                          uint8_t value)
{
}
void MidiInput::Observer::onProgramChange(uint8_t channel, uint8_t program) {}
void MidiInput::Observer::onChannelPressureChange(uint8_t channel, uint8_t value) {}
void MidiInput::Observer::onPitchBendChange(uint8_t channel, uint16_t value) {}
