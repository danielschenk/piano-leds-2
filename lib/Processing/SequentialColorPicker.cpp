#include "SequentialColorPicker.hpp"

namespace processing
{

SequentialColorPicker::SequentialColorPicker(std::vector<RgbColor> sequence)
    : sequence(std::move(sequence))
{
}

RgbColor SequentialColorPicker::pick()
{
    if (currentColor >= sequence.cend() || currentColor < sequence.cbegin())
        currentColor = sequence.cbegin();
    return *currentColor++;
}

}  // namespace processing
