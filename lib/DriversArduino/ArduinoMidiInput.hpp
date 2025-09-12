#ifndef DRIVERS_ARDUINO_ARDUINOMIDIINPUT_H_
#define DRIVERS_ARDUINO_ARDUINOMIDIINPUT_H_

#include "BaseMidiInput.hpp"

class Stream;

/**
 * MIDI input implementation which uses Arduino Serial.
 */
class ArduinoMidiInput : public BaseMidiInput
{
  public:
    /**
     * Constructor.
     *
     * @param serial   The Arduino serial port driver to use.
     */
    explicit ArduinoMidiInput(Stream& serial);

    /**
     * Destructor.
     */
    virtual ~ArduinoMidiInput() = default;

    // Prevent implicit default constructors and assignment operator.
    ArduinoMidiInput() = delete;
    ArduinoMidiInput(const ArduinoMidiInput&) = delete;
    ArduinoMidiInput& operator=(const ArduinoMidiInput&) = delete;

    /**
     * Run the serial driver and process available data.
     */
    void run();

    // MidiInterface implementation
    virtual unsigned int getPortCount() const;
    virtual void openPort(int number);

  private:
    Stream& serial;
};

#endif /* DRIVERS_ARDUINO_ARDUINOMIDIINPUT_H_ */
