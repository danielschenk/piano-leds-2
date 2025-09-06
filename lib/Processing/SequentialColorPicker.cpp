#include "SequentialColorPicker.hpp"

namespace Processing
{

using namespace ColorValue;

const std::array<TRgb, 6> SequentialColorPicker::sequence{red, green, blue, yellow, magenta, cyan};

TRgb SequentialColorPicker::pick()
{
    auto color = *currentColor;
    if (++currentColor >= sequence.cend())
        currentColor = sequence.cbegin();
    return color;
}

}
