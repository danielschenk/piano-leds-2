#ifndef PROCESSING_LINEARRGBFUNCTION_H_
#define PROCESSING_LINEARRGBFUNCTION_H_

#include "RgbFunction.hpp"
#include "ProcessingTypes.hpp"

/**
 * Function which describes a time invariant linear relation between note state and RGB output.
 */
class LinearRgbFunction : public RgbFunction
{
  public:
    LinearRgbFunction() = default;
    explicit LinearRgbFunction(const processing::RgbColor& color);

    // defaults result in full white for max MIDI velocity
    processing::LinearConstants redConstants = {2, 1};
    processing::LinearConstants greenConstants = {2, 1};
    processing::LinearConstants blueConstants = {2, 1};

    // RgbFunction implementation
    processing::RgbColor calculate(const processing::NoteState& noteState,
                                   processing::Timestamp currentTime) const override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

  protected:
    // RgbFunction implementation
    std::string getObjectType() const override;

    static constexpr const char* rFactorJsonKey = "rFactor";
    static constexpr const char* gFactorJsonKey = "gFactor";
    static constexpr const char* bFactorJsonKey = "bFactor";
    static constexpr const char* rOffsetJsonKey = "rOffset";
    static constexpr const char* gOffsetJsonKey = "gOffset";
    static constexpr const char* bOffsetJsonKey = "bOffset";
};

#endif /* PROCESSING_LINEARRGBFUNCTION_H_ */
