#include "MidiTask.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "ArduinoMidiInput.hpp"
#include "Logging.hpp"

#define LOGGING_COMPONENT "MidiTask"

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

#ifdef DIAGNOSTICS
    static uint32_t tickCount = 0;
    tickCount++;
    if ((tickCount % 3000) == 0)  // roughly every ~30s if wake ~10ms
    {
        auto d = midiInput.getDiagnostics();
        LOG_INFO_PARAMS("MIDI diag: bytes=%lu msgs=%lu unsup=%lu parseErr=%lu maxSize=%u",
                        (unsigned long)d.totalBytes, (unsigned long)d.totalMessages,
                        (unsigned long)d.unsupportedStatus, (unsigned long)d.parseErrors,
                        (unsigned int)d.maxMessageSize);

        UBaseType_t hwm = uxTaskGetStackHighWaterMark(NULL);
        LOG_INFO_PARAMS("MIDI diag: stackHWM=%lu words", (unsigned long)hwm);
    }
#endif
}
