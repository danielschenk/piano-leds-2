#ifndef COMMON_UTILITIES_MIDIMESSAGELOGGER_H_
#define COMMON_UTILITIES_MIDIMESSAGELOGGER_H_

#include "MidiInput.hpp"

class MidiInput;

/**
 * Class which logs a description of incoming MIDI messages.
 */
class MidiMessageLogger : public MidiInput::Observer
{
  public:
    /**
     * Constructor.
     *
     * @param midiInput The MIDI input to use
     */
    explicit MidiMessageLogger(MidiInput& midiInput);

    /**
     * Destructor.
     */
    virtual ~MidiMessageLogger();

    // Prevent implicit constructors and assignment
    MidiMessageLogger() = delete;
    MidiMessageLogger(const MidiMessageLogger&) = delete;
    MidiMessageLogger& operator=(const MidiMessageLogger&) = delete;

    // MidiInput::Observer implementation
    virtual void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on);
    virtual void onProgramChange(uint8_t channel, uint8_t program);
    virtual void onControlChange(uint8_t channel, MidiInterface::ControllerNumber controller,
                                 uint8_t value);
    virtual void onChannelPressureChange(uint8_t channel, uint8_t value);
    virtual void onPitchBendChange(uint8_t channel, uint16_t value);

  private:
    MidiInput& midiInput;
};

#endif /* COMMON_UTILITIES_MIDIMESSAGELOGGER_H_ */
