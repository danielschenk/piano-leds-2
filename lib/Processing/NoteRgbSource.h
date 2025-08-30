#ifndef PROCESSING_NOTERGBSOURCE_H_
#define PROCESSING_NOTERGBSOURCE_H_

#include "IMidiInput.h"
#include "Scheduler.h"
#include "IProcessingBlock.h"

#include <mutex>
#include <array>
#include <memory>

class IRgbFunction;
class IRgbFunctionFactory;
class ITime;

/**
 * RGB source which generates RGB data based on note on/off events.
 */
class NoteRgbSource
    : public IProcessingBlock
    , public IMidiInput::IObserver
{
public:
    /**
     * Constructor.
     *
     * @param midiInput             MIDI input to subscribe to incoming notes.
     * @param rgbFunctionFactory    The factory needed to construct the RGB function from JSON.
     * @param time                  The time provider to get the execution time and pass it to the RGB function.
     *
     */
    NoteRgbSource(IMidiInput& midiDriver,
                  const IRgbFunctionFactory& rgbFunctionFactory,
                  const ITime& time);

    ~NoteRgbSource() override;

    NoteRgbSource(NoteRgbSource&) = delete;
    NoteRgbSource& operator=(NoteRgbSource&) = delete;

    // IProcessingBlock implementation.
    void activate() override;
    void deactivate() override;
    void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap) override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

    uint8_t getChannel() const;
    void setChannel(uint8_t channel);
    bool isUsingPedal() const;
    void setUsingPedal(bool usingPedal);

    void setRgbFunction(std::shared_ptr<IRgbFunction> rgbFunction);

    // IMidiInput::IObserver implementation
    void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) override;
    void onControlChange(uint8_t channel, IMidiInput::TControllerNumber controller, uint8_t value) override;
    void onProgramChange(uint8_t channel, uint8_t program) override;
    void onChannelPressureChange(uint8_t channel, uint8_t value) override;
    void onPitchBendChange(uint8_t channel, uint16_t value) override;


protected:
    // IProcessingBlock implementation
    std::string getObjectType() const override;

private:
    static constexpr const char* usingPedalJsonKey    = "usingPedal";
    static constexpr const char* channelJsonKey       = "channel";
    static constexpr const char* rgbFunctionJsonKey   = "rgbFunction";

    mutable std::mutex mutex;
    bool active = false;
    bool usingPedal = true;
    const IRgbFunctionFactory& rgbFunctionFactory;
    IMidiInput& midiInput;
    uint8_t channel = 0;
    Scheduler scheduler;
    std::array<Processing::TNoteState, IMidiInterface::numNotes> noteStates;
    bool pedalPressed;
    std::shared_ptr<IRgbFunction> rgbFunction;
    const ITime& time;
};

#endif /* PROCESSING_NOTERGBSOURCE_H_ */
