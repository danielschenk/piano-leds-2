#ifndef COMMON_UTILITIES_MIDIMESSAGELOGGER_H_
#define COMMON_UTILITIES_MIDIMESSAGELOGGER_H_

#include "IMidiInput.h"

class IMidiInput;

/**
 * Class which logs a description of incoming MIDI messages.
 */
class MidiMessageLogger : public IMidiInput::IObserver
{
  public:
    /**
     * Constructor.
     *
     * @param midiInput The MIDI input to use
     */
    explicit MidiMessageLogger(IMidiInput& midiInput);

    /**
     * Destructor.
     */
    virtual ~MidiMessageLogger();

    // Prevent implicit constructors and assignment
    MidiMessageLogger() = delete;
    MidiMessageLogger(const MidiMessageLogger&) = delete;
    MidiMessageLogger& operator=(const MidiMessageLogger&) = delete;

    // IMidiInput::IObserver implementation
    virtual void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on);
    virtual void onProgramChange(uint8_t channel, uint8_t program);
    virtual void onControlChange(uint8_t channel, IMidiInterface::TControllerNumber controller,
                                 uint8_t value);
    virtual void onChannelPressureChange(uint8_t channel, uint8_t value);
    virtual void onPitchBendChange(uint8_t channel, uint16_t value);

  private:
    IMidiInput& midiInput;
};

#endif /* COMMON_UTILITIES_MIDIMESSAGELOGGER_H_ */
