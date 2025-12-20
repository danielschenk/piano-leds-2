#ifndef PROCESSING_NOTEVISUALIZER_HPP
#define PROCESSING_NOTEVISUALIZER_HPP

#include <memory>

#include "ProcessingBlock.hpp"

namespace processing
{
class RgbFunction;
class ColorPicker;
}  // namespace processing

class NoteVisualizer : public ProcessingBlock
{
  public:
    NoteVisualizer() = default;

    // ProcessingBlock implementation
    void execute(processing::RgbStrip& strip, const Input& input) override;

    void setRgbFunction(std::shared_ptr<processing::RgbFunction> rgbFunction);

  protected:
    // ProcessingBlock implementation
    std::string getObjectType() const override;

  private:
    static constexpr const char* usingPedalJsonKey = "usingPedal";
    static constexpr const char* channelJsonKey = "channel";
    static constexpr const char* rgbFunctionJsonKey = "rgbFunction";

    std::shared_ptr<processing::RgbFunction> rgbFunction;
};

#endif /* PROCESSING_NOTEVISUALIZER_HPP */
