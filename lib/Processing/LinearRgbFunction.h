#ifndef PROCESSING_LINEARRGBFUNCTION_H_
#define PROCESSING_LINEARRGBFUNCTION_H_

#include "IRgbFunction.h"
#include "ProcessingTypes.h"

/**
 * Function which describes a time invariant linear relation between note state and RGB output.
 */
class LinearRgbFunction
    : public IRgbFunction
{
public:
    LinearRgbFunction() = default;
    explicit LinearRgbFunction(const Processing::TRgb& color);

    void setRedConstants(Processing::TLinearConstants redConstants);
    void setGreenConstants(Processing::TLinearConstants greenConstants);
    void setBlueConstants(Processing::TLinearConstants blueConstants);

    Processing::TLinearConstants getRedConstants() const;
    Processing::TLinearConstants getGreenConstants() const;
    Processing::TLinearConstants getBlueConstants() const;

    // IRgbFunction implementation
    Processing::TRgb calculate(const Processing::TNoteState& noteState, Processing::TTime currentTime) const override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

protected:
    // IRgbFunction implementation
    std::string getObjectType() const override;

private:
    /**
     * The constants.
     * The defaults are chosen with the maximum MIDI velocity in mind (127), which will result in a value of
     * 255 (maximum LED value for most drivers).
     */
    Processing::TLinearConstants redConstants = {2, 1};
    Processing::TLinearConstants greenConstants = {2, 1};
    Processing::TLinearConstants blueConstants = {2, 1};

    static constexpr const char* rFactorJsonKey = "rFactor";
    static constexpr const char* gFactorJsonKey = "gFactor";
    static constexpr const char* bFactorJsonKey = "bFactor";
    static constexpr const char* rOffsetJsonKey = "rOffset";
    static constexpr const char* gOffsetJsonKey = "gOffset";
    static constexpr const char* bOffsetJsonKey = "bOffset";
};

#endif /* PROCESSING_LINEARRGBFUNCTION_H_ */
