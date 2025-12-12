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
    RgbColor pick() override;

    std::vector<RgbColor> sequence{color_constants::red,     color_constants::green,
                                   color_constants::blue,    color_constants::yellow,
                                   color_constants::magenta, color_constants::cyan};

  private:
    std::size_t index{0};
};

}  // namespace processing

#endif
