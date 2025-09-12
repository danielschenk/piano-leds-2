#ifndef PROCESSING_SEQUENTIALCOLORPICKER_HPP
#define PROCESSING_SEQUENTIALCOLORPICKER_HPP

#include <array>

#include "ColorPicker.hpp"

namespace processing
{

class SequentialColorPicker : public ColorPicker
{
  public:
    SequentialColorPicker() = default;
    RgbColor pick();

  private:
    static const std::array<RgbColor, 6> sequence;
    decltype(sequence)::const_iterator currentColor{sequence.cbegin()};
};

}  // namespace processing

#endif
