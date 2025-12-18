#ifndef PROCESSING_ONOFFRGBFUNCTION_HPP
#define PROCESSING_ONOFFRGBFUNCTION_HPP

#include "RgbFunction.hpp"

namespace processing
{

class OnOffRgbFunction : public RgbFunction
{
  public:
    OnOffRgbFunction() = default;
    explicit OnOffRgbFunction(const RgbColor& color);

    RgbColor color;

    // RgbFunction implementation
    RgbColor calculate(const NoteState& noteState, Timestamp currentTime) const override;
};

}  // namespace processing

#endif
