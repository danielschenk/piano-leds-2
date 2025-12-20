#include "OnOffRgbFunction.hpp"

namespace processing
{

OnOffRgbFunction::OnOffRgbFunction(const RgbColor& color) : color(color) {}

RgbColor OnOffRgbFunction::calculate(const NoteState& noteState, Timestamp currentTime) const
{
    if (noteState.sounding)
    {
        if (noteState.pressDownColor != color_constants::off)
            return noteState.pressDownColor;
        else
            return color;
    }
    else
    {
        return color_constants::off;
    }
}

}  // namespace processing
