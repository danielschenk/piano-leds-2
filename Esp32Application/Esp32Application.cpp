#include "ArduinoMidiInput.hpp"
#include "ConcertInfrastructure.hpp"
#include "Logging.hpp"
#include "LoggingTask.hpp"
#include "MidiMessageLogger.hpp"
#include "MidiTask.hpp"
#include "ProcessingTask.hpp"
#if PIANOLEDS_NEOPIXEL
#include "LedTaskNeoPixel.hpp"
#else
#include "LedTask.hpp"
#endif
#include "Board.hpp"
#include "FreeRtosMonotonicTime.hpp"
#include "NetworkTask.hpp"
#include "SystemSettingsModel.hpp"
#include "bootloader_random.h"

#define LOGGING_COMPONENT "Esp32Application"

static MidiTask* midiTaskPtr{nullptr};

static constexpr uint32_t defaultStackSize(4096);

enum
{
    PRIORITY_IDLE = 0,
    PRIORITY_LOW = 1,
    PRIORITY_UI = 2,
    PRIORITY_CRITICAL = 3
};

static void startNetwork();

void setup()
{
    static FreeRtosMonotonicTime freeRtosTime;

    LoggingEntryPoint::setTime(&freeRtosTime);
    Serial.begin(115200, SERIAL_8N1, DEBUG_RX_PIN, DEBUG_TX_PIN);
    static LoggingTask loggingTask(Serial, defaultStackSize, PRIORITY_LOW);

    LOG_INFO("MIDI-LED-Controller (MLC) (c) Daniel Schenk, 2017-2025");
    LOG_INFO("initializing application...");

    pinMode(RUN_LED_PIN, OUTPUT);
    digitalWrite(RUN_LED_PIN, 0);

    static ArduinoMidiInput midiInput(Serial2);
    static MidiTask midiTask(midiInput, defaultStackSize, PRIORITY_CRITICAL);
    // Required by Serial2 callback. TODO find better solution to this
    midiTaskPtr = &midiTask;
    Serial2.begin(31250, SERIAL_8N1, MIDI_RX_PIN, MIDI_TX_PIN);

    static MidiMessageLogger midiMessageLogger(midiInput);

    bootloader_random_enable();

    static application::ConcertInfrastructure concertInfrastructure(midiInput, freeRtosTime);
    static ProcessingTask processingTask(concertInfrastructure.concert, defaultStackSize,
                                         PRIORITY_CRITICAL);

#if PIANOLEDS_NEOPIXEL
    static LedTaskNeoPixel ledTask(concertInfrastructure.concert, LED_DATA_PIN, defaultStackSize,
                                   PRIORITY_CRITICAL);
#else
    static LedTask ledTask(concertInfrastructure.concert, LED_DATA_PIN, LED_CLOCK_PIN,
                           defaultStackSize, PRIORITY_CRITICAL);
#endif

    if (false)
        startNetwork();

    LOG_INFO("initialization done");
}

static void startNetwork()
{
    static SystemSettingsModel systemSettingsModel;
    static NetworkTask networkTask(systemSettingsModel, defaultStackSize, PRIORITY_LOW);
}

void loop()
{
    static unsigned loopCount(0);

    vTaskDelay(1000);
    digitalWrite(RUN_LED_PIN, !digitalRead(RUN_LED_PIN));

    if ((loopCount % (5 * 60)) == 0)
    {
        LOG_INFO_PARAMS("free heap: %u", ESP.getFreeHeap());
    }

    ++loopCount;
}

void serialEvent2()
{
    midiTaskPtr->wake();
}
