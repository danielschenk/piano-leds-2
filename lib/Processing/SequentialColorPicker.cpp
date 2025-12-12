#include "SequentialColorPicker.hpp"

namespace processing
{

RgbColor SequentialColorPicker::pick()
{
    if (index >= sequence.size())
        index = 0;
    return sequence[index++];
}

}  // namespace processing
