#ifndef PROCESSING_NOTEVISUALIZER_HPP
#define PROCESSING_NOTEVISUALIZER_HPP

#include <memory>
#include <mutex>
#include <vector>

#include "ProcessingBlock.hpp"

class MonotonicTime;
class NoteStateTracker;

namespace processing
{
class RgbFunction;
class ColorPicker;
}  // namespace processing

class NoteVisualizer : public ProcessingBlock
{
  public:
    NoteVisualizer(NoteStateTracker& noteStateTracker, const MonotonicTime& time);

    NoteVisualizer(NoteVisualizer&) = delete;
    NoteVisualizer& operator=(NoteVisualizer&) = delete;

    // ProcessingBlock implementation
    void execute(processing::RgbStrip& strip, const NoteStates& noteStates,
                 const processing::NoteToLightMap& noteToLightMap) override;

    void setRgbFunction(std::shared_ptr<processing::RgbFunction> rgbFunction);

  protected:
    // ProcessingBlock implementation
    std::string getObjectType() const override;

  private:
    static constexpr const char* usingPedalJsonKey = "usingPedal";
    static constexpr const char* channelJsonKey = "channel";
    static constexpr const char* rgbFunctionJsonKey = "rgbFunction";

    mutable std::mutex mutex;
    NoteStateTracker& noteStateTracker;
    const MonotonicTime& time;
    std::shared_ptr<processing::RgbFunction> rgbFunction;
};

#endif /* PROCESSING_NOTEVISUALIZER_HPP */
