#ifndef ESP32APPLICATION_MIDITASK_H_
#define ESP32APPLICATION_MIDITASK_H_

#include "BaseTask.hpp"

class ArduinoMidiInput;

/**
 * FreeRTOS task processing incoming MIDI bytes.
 */
class MidiTask : public BaseTask
{
  public:
    /**
     * Constructor.
     *
     * @param midiInput     The midi input to use
     * @param stackSize     Stack size in words
     * @param priority      Priority
     */
    MidiTask(ArduinoMidiInput& midiInput, uint32_t stackSize, UBaseType_t priority);

    /**
     * Destructor.
     */
    ~MidiTask() override = default;

    // Prevent implicit default constructors and assignment operator.
    MidiTask() = delete;
    MidiTask(const MidiTask&) = delete;
    MidiTask& operator=(const MidiTask&) = delete;

    /**
     * Wake up the thread.
     */
    void wake();

  private:
    void run() override;

    ArduinoMidiInput& midiInput;
};

#endif /* ESP32APPLICATION_MIDITASK_H_ */
