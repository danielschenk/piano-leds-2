#ifndef PROCESSING_RGBFUNCTION_HPP
#define PROCESSING_RGBFUNCTION_HPP

#include "JsonConvertible.hpp"
#include "ProcessingTypes.hpp"

class RgbFunction : public JsonConvertible
{
  public:
    static constexpr const char* jsonTypeNameLinearRgbFunction = "LinearRgbFunction";
    static constexpr const char* jsonTypeNamePianoDecayRgbFunction = "PianoDecayRgbFunction";

    virtual ~RgbFunction() = default;
    virtual processing::RgbColor calculate(const processing::NoteState& noteState,
                                           processing::Timestamp currentTime) const = 0;
};

#endif /* PROCESSING_RGBFUNCTION_HPP */
