#include "OnOffRgbFunction.hpp"

namespace processing
{

OnOffRgbFunction::OnOffRgbFunction(const RgbColor& color) : color(color) {}

RgbColor OnOffRgbFunction::calculate(const NoteState& noteState, Timestamp currentTime) const
{
    if (noteState.sounding)
        return color;
    else
        return color_constants::off;
}

}  // namespace processing
