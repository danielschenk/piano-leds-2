#ifndef PROCESSING_LINEARRGBFUNCTION_HPP
#define PROCESSING_LINEARRGBFUNCTION_HPP

#include "ProcessingTypes.hpp"
#include "RgbFunction.hpp"

namespace processing
{

/**
 * Function which describes a time invariant linear relation between note state and RGB output.
 */
class LinearRgbFunction : public RgbFunction
{
  public:
    LinearRgbFunction() = default;
    explicit LinearRgbFunction(const RgbColor& color);

    // defaults result in full white for max MIDI velocity
    LinearConstants redConstants = {2, 1};
    LinearConstants greenConstants = {2, 1};
    LinearConstants blueConstants = {2, 1};

    // RgbFunction implementation
    RgbColor calculate(const NoteState& noteState, Timestamp currentTime) const override;
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

}  // namespace processing

#endif /* PROCESSING_LINEARRGBFUNCTION_HPP */
