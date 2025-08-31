#include "ArduinoMidiInput.h"
#include "LoggingTask.h"
#include "Logging.h"
#include "MidiMessageLogger.h"

#include "MidiTask.h"

#include "ConcertInfrastructure.h"
#include "IPatch.h"
#include "EqualRangeRgbSource.h"
#include "NoteRgbSource.h"
#include "LinearRgbFunction.h"
#include "PianoDecayRgbFunction.h"
#include "ProcessingTask.h"
#include "LedTask.h"
#include "SystemSettingsModel.h"
#include "NetworkTask.h"

#include "Board.h"
#include "FreeRtosTime.h"

#define LOGGING_COMPONENT "Esp32Application"

static MidiTask* midiTaskPtr(nullptr);

static constexpr uint32_t defaultStackSize(4096);

enum
{
    /**
     * Idle priority level.
     */
    PRIORITY_IDLE = 0,

    /**
     * Priority level for everything which is least important.
     * @note Same as the task which runs the setup/loop functions.
     * Examples: logging
     */
    PRIORITY_LOW = 1,

    /**
     * Priority level for things which are not critical, but should not be delayed by logging.
     * Examples: web UI for configuration
     * */
    PRIORITY_UI = 2,

    /**
     * Priority level for the main tasks of the device which should always have priority.
     * Examples: MIDI, processing, SPI
     */
    PRIORITY_CRITICAL = 3
};

static void startNetwork();

void setup()
{
    static FreeRtosTime freeRtosTime;

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

    static application::ConcertInfrastructure concertInfrastructure(midiInput, freeRtosTime);

    // TODO read concert from storage
    // For now, add something to test with.
    auto& concert = concertInfrastructure.concert;
    IPatch* patch(concert.getPatch(concert.addPatch()));
    patch->setName("whiteOnBlue");
    patch->setBank(0);
    patch->setProgram(19);

    // Add constant blue background
    auto src1(new EqualRangeRgbSource);
    src1->setColor(Processing::TRgb({0, 0, 32}));
    patch->getProcessingChain().insertBlock(src1);

    // Full white for any sounding key
    auto src2(new NoteRgbSource(midiInput,
                                concertInfrastructure.rgbFunctionFactory,
                                freeRtosTime));
    auto rgbFunction(std::make_shared<LinearRgbFunction>());
    const Processing::TLinearConstants fullWhite({255, 0});
    rgbFunction->setRedConstants(fullWhite);
    rgbFunction->setGreenConstants(fullWhite);
    rgbFunction->setBlueConstants(fullWhite);
    src2->setRgbFunction(rgbFunction);
    src2->setUsingPedal(true);
    patch->getProcessingChain().insertBlock(src2);
    patch->activate();

    // Add another patch
    IPatch* patch2(concert.getPatch(concert.addPatch()));
    auto src3(new NoteRgbSource(midiInput,
                                concertInfrastructure.rgbFunctionFactory,
                                freeRtosTime));

    // Sounding notes become blue, intensity is the velocity of the note multiplied by 2
    rgbFunction = std::make_shared<LinearRgbFunction>();
    rgbFunction->setBlueConstants({2, 0});
    src3->setRgbFunction(rgbFunction);

    src3->setUsingPedal(true);

    patch2->getProcessingChain().insertBlock(src3);
    patch2->setName("blueNote");
    patch2->setBank(0);
    patch2->setProgram(20);

    // Add another patch
    IPatch* patch3(concert.getPatch(concert.addPatch()));

    // Red background, white notes, mimic piano
    auto src4(new EqualRangeRgbSource);
    src4->setColor({32, 0, 0});
    patch3->getProcessingChain().insertBlock(src4);

    auto src5(new NoteRgbSource(midiInput,
                                concertInfrastructure.rgbFunctionFactory,
                                freeRtosTime));

    auto fnc(std::make_shared<PianoDecayRgbFunction>());
    src5->setRgbFunction(fnc);
    src5->setUsingPedal(true);
    patch3->getProcessingChain().insertBlock(src5);
    patch3->setName("whitePianoNotes");
    patch3->setBank(0);
    patch3->setProgram(21);

    static ProcessingTask processingTask(concert, defaultStackSize, PRIORITY_CRITICAL);

    static LedTask ledTask(concert, LED_DATA_PIN, LED_CLOCK_PIN, defaultStackSize,
        PRIORITY_CRITICAL);

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
    static unsigned s_loopCount(0);

    // Nothing to do, leave everything to the other tasks.
    vTaskDelay(1000);

    // Blink to indicate we're alive.
    digitalWrite(RUN_LED_PIN, !digitalRead(RUN_LED_PIN));

    if((s_loopCount % (5 * 60)) == 0)
    {
        LOG_INFO_PARAMS("free heap: %u", ESP.getFreeHeap());
    }

    ++s_loopCount;
}

void serialEvent2()
{
    midiTaskPtr->wake();
}
