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

    std::shared_ptr<processing::RgbFunction> rgbFunction;

  protected:
    // ProcessingBlock implementation
    std::string getObjectType() const override;

  private:
    MidiInterface::Channel channel{0};
};

#endif /* PROCESSING_NOTEVISUALIZER_HPP */
