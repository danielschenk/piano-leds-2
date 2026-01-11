#ifndef PROCESSING_NOTEVISUALIZER_HPP
#define PROCESSING_NOTEVISUALIZER_HPP

#include <cstddef>
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
    bool valid(const Input& input) override;
    void execute(processing::RgbStrip& strip, const Input& input) override;

    std::shared_ptr<processing::RgbFunction> rgbFunction;
    std::size_t midiSlot{0};

  protected:
    // ProcessingBlock implementation
    std::string getObjectType() const override;
};

#endif /* PROCESSING_NOTEVISUALIZER_HPP */
