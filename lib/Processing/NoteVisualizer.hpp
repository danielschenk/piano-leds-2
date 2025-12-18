#ifndef PROCESSING_NOTEVISUALIZER_H_
#define PROCESSING_NOTEVISUALIZER_H_

#include <array>
#include <atomic>
#include <memory>
#include <mutex>

#include "MidiInput.hpp"
#include "ProcessingBlock.hpp"
#include "Scheduler.hpp"

class MonotonicTime;

namespace processing
{
class RgbFunction;
class IRgbFunctionFactory;
class ColorPicker;
}  // namespace processing

class NoteVisualizer : public ProcessingBlock, public MidiInput::Observer
{
  public:
    NoteVisualizer(MidiInput& midiDriver, const processing::IRgbFunctionFactory& rgbFunctionFactory,
                   const MonotonicTime& time);

    ~NoteVisualizer() override;

    NoteVisualizer(NoteVisualizer&) = delete;
    NoteVisualizer& operator=(NoteVisualizer&) = delete;

    // ProcessingBlock implementation
    void activate() override;
    void deactivate() override;
    void execute(processing::RgbStrip& strip,
                 const processing::NoteToLightMap& noteToLightMap) override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

    uint8_t getChannel() const;
    void setChannel(uint8_t channel);
    bool isUsingPedal() const;
    void setUsingPedal(bool usingPedal);

    void setRgbFunction(std::shared_ptr<processing::RgbFunction> rgbFunction);
    void setPressDownColorPicker(std::shared_ptr<processing::ColorPicker> colorPicker);

    // MidiInput::Observer implementation
    void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) override;
    void onControlChange(uint8_t channel, MidiInput::ControllerNumber controller,
                         uint8_t value) override;

  protected:
    // ProcessingBlock implementation
    std::string getObjectType() const override;

  private:
    static constexpr const char* usingPedalJsonKey = "usingPedal";
    static constexpr const char* channelJsonKey = "channel";
    static constexpr const char* rgbFunctionJsonKey = "rgbFunction";

    mutable std::mutex mutex;
    std::atomic<bool> active{false};
    bool usingPedal{true};
    const processing::IRgbFunctionFactory& rgbFunctionFactory;
    MidiInput& midiInput;
    uint8_t channel = 0;
    Scheduler scheduler;
    std::array<processing::NoteState, MidiInterface::numNotes> noteStates;
    bool pedalPressed{false};
    std::shared_ptr<processing::RgbFunction> rgbFunction;
    std::shared_ptr<processing::ColorPicker> pressDownColorPicker;
    const MonotonicTime& time;
};

#endif /* PROCESSING_NOTEVISUALIZER_H_ */
