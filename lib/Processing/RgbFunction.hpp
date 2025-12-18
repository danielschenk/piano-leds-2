#ifndef PROCESSING_RGBFUNCTION_HPP
#define PROCESSING_RGBFUNCTION_HPP

#include "JsonConvertible.hpp"
#include "ProcessingTypes.hpp"

namespace processing
{

class RgbFunction : public JsonConvertible
{
  public:
    static constexpr const char* jsonTypeNameLinearRgbFunction = "LinearRgbFunction";
    static constexpr const char* jsonTypeNamePianoDecayRgbFunction = "PianoDecayRgbFunction";

    virtual ~RgbFunction() = default;
    virtual RgbColor calculate(const NoteState& noteState, Timestamp currentTime) const = 0;
};

}  // namespace processing

#endif /* PROCESSING_RGBFUNCTION_HPP */
