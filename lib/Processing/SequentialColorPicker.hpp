#ifndef PROCESSING_SEQUENTIALCOLORPICKER_HPP
#define PROCESSING_SEQUENTIALCOLORPICKER_HPP

#include <vector>

#include "Color.hpp"
#include "ColorPicker.hpp"

namespace processing
{

class SequentialColorPicker : public ColorPicker
{
  public:
    SequentialColorPicker() = default;
    explicit SequentialColorPicker(std::vector<RgbColor> sequence);
    RgbColor pick() override;

  private:
    std::vector<RgbColor> sequence{color_constants::red,     color_constants::green,
                                   color_constants::blue,    color_constants::yellow,
                                   color_constants::magenta, color_constants::cyan};
    decltype(sequence)::const_iterator currentColor{sequence.cbegin()};
};

}  // namespace processing

#endif
