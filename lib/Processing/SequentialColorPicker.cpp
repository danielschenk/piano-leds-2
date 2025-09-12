#include "SequentialColorPicker.hpp"

namespace processing
{

using namespace color_constants;

const std::array<RgbColor, 6> SequentialColorPicker::sequence{red,    green,   blue,
                                                              yellow, magenta, cyan};

RgbColor SequentialColorPicker::pick()
{
    auto color = *currentColor;
    if (++currentColor >= sequence.cend())
        currentColor = sequence.cbegin();
    return color;
}

}  // namespace processing
