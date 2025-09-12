#include "MidiTask.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "ArduinoMidiInput.h"

MidiTask::MidiTask(ArduinoMidiInput& midiInput, uint32_t stackSize, UBaseType_t priority)
    : BaseTask(), midiInput(midiInput)
{
    start("midi", stackSize, priority);
}

void MidiTask::wake()
{
    xTaskNotifyGive(getTaskHandle());
}

void MidiTask::run()
{
    // TODO workaround for lack of serialEvent implementation on ESP32.
    // Wait for event for a maximum amount of time.
    // This effectively is a polling implementation instead of an event-driven one.
    ulTaskNotifyTake(pdTRUE, 10);

    // Process any data.
    midiInput.run();
}
