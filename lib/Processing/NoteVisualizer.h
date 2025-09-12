#ifndef PROCESSING_NOTEVISUALIZER_H_
#define PROCESSING_NOTEVISUALIZER_H_

#include <array>
#include <memory>
#include <mutex>

#include "IMidiInput.h"
#include "ProcessingBlock.h"
#include "Scheduler.h"

class IRgbFunction;
class IRgbFunctionFactory;
class ITime;
namespace Processing
{
class ColorPicker;
}

class NoteVisualizer : public ProcessingBlock, public IMidiInput::IObserver
{
  public:
    NoteVisualizer(IMidiInput& midiDriver, const IRgbFunctionFactory& rgbFunctionFactory,
                   const ITime& time);

    ~NoteVisualizer() override;

    NoteVisualizer(NoteVisualizer&) = delete;
    NoteVisualizer& operator=(NoteVisualizer&) = delete;

    // ProcessingBlock implementation
    void activate() override;
    void deactivate() override;
    void execute(Processing::TRgbStrip& strip,
                 const Processing::TNoteToLightMap& noteToLightMap) override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

    uint8_t getChannel() const;
    void setChannel(uint8_t channel);
    bool isUsingPedal() const;
    void setUsingPedal(bool usingPedal);

    void setRgbFunction(std::shared_ptr<IRgbFunction> rgbFunction);
    void setPressDownColorPicker(std::shared_ptr<Processing::ColorPicker> colorPicker);

    // IMidiInput::IObserver implementation
    void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) override;
    void onControlChange(uint8_t channel, IMidiInput::TControllerNumber controller,
                         uint8_t value) override;
    void onProgramChange(uint8_t channel, uint8_t program) override;
    void onChannelPressureChange(uint8_t channel, uint8_t value) override;
    void onPitchBendChange(uint8_t channel, uint16_t value) override;

  protected:
    // ProcessingBlock implementation
    std::string getObjectType() const override;

  private:
    static constexpr const char* usingPedalJsonKey = "usingPedal";
    static constexpr const char* channelJsonKey = "channel";
    static constexpr const char* rgbFunctionJsonKey = "rgbFunction";

    mutable std::mutex mutex;
    bool active{false};
    bool usingPedal{true};
    const IRgbFunctionFactory& rgbFunctionFactory;
    IMidiInput& midiInput;
    uint8_t channel = 0;
    Scheduler scheduler;
    std::array<Processing::TNoteState, IMidiInterface::numNotes> noteStates;
    bool pedalPressed{false};
    std::shared_ptr<IRgbFunction> rgbFunction;
    std::shared_ptr<Processing::ColorPicker> pressDownColorPicker;
    const ITime& time;
};

#endif /* PROCESSING_NOTEVISUALIZER_H_ */
