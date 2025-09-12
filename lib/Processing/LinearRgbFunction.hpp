#ifndef PROCESSING_LINEARRGBFUNCTION_H_
#define PROCESSING_LINEARRGBFUNCTION_H_

#include "IRgbFunction.hpp"
#include "ProcessingTypes.hpp"

/**
 * Function which describes a time invariant linear relation between note state and RGB output.
 */
class LinearRgbFunction : public IRgbFunction
{
  public:
    LinearRgbFunction() = default;
    explicit LinearRgbFunction(const processing::RgbColor& color);

    void setRedConstants(processing::TLinearConstants redConstants);
    void setGreenConstants(processing::TLinearConstants greenConstants);
    void setBlueConstants(processing::TLinearConstants blueConstants);

    processing::TLinearConstants getRedConstants() const;
    processing::TLinearConstants getGreenConstants() const;
    processing::TLinearConstants getBlueConstants() const;

    // IRgbFunction implementation
    processing::RgbColor calculate(const processing::TNoteState& noteState,
                                   processing::TTime currentTime) const override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

  protected:
    // IRgbFunction implementation
    std::string getObjectType() const override;

  private:
    /**
     * The constants.
     * The defaults are chosen with the maximum MIDI velocity in mind (127), which will result in a
     * value of 255 (maximum LED value for most drivers).
     */
    processing::TLinearConstants redConstants = {2, 1};
    processing::TLinearConstants greenConstants = {2, 1};
    processing::TLinearConstants blueConstants = {2, 1};

    static constexpr const char* rFactorJsonKey = "rFactor";
    static constexpr const char* gFactorJsonKey = "gFactor";
    static constexpr const char* bFactorJsonKey = "bFactor";
    static constexpr const char* rOffsetJsonKey = "rOffset";
    static constexpr const char* gOffsetJsonKey = "gOffset";
    static constexpr const char* bOffsetJsonKey = "bOffset";
};

#endif /* PROCESSING_LINEARRGBFUNCTION_H_ */
