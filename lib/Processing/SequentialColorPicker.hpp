#ifndef PROCESSING_SEQUENTIALCOLORPICKER_HPP
#define PROCESSING_SEQUENTIALCOLORPICKER_HPP

#include <array>

#include "ColorPicker.hpp"

namespace Processing
{

class SequentialColorPicker : public ColorPicker
{
  public:
    SequentialColorPicker() = default;
    TRgb pick();

  private:
    static const std::array<TRgb, 6> sequence;
    decltype(sequence)::const_iterator currentColor{sequence.cbegin()};
};

}  // namespace Processing

#endif
