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

    void setRedConstants(processing::LinearConstants redConstants);
    void setGreenConstants(processing::LinearConstants greenConstants);
    void setBlueConstants(processing::LinearConstants blueConstants);

    processing::LinearConstants getRedConstants() const;
    processing::LinearConstants getGreenConstants() const;
    processing::LinearConstants getBlueConstants() const;

    // IRgbFunction implementation
    processing::RgbColor calculate(const processing::NoteState& noteState,
                                   processing::Timestamp currentTime) const override;
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
    processing::LinearConstants redConstants = {2, 1};
    processing::LinearConstants greenConstants = {2, 1};
    processing::LinearConstants blueConstants = {2, 1};

    static constexpr const char* rFactorJsonKey = "rFactor";
    static constexpr const char* gFactorJsonKey = "gFactor";
    static constexpr const char* bFactorJsonKey = "bFactor";
    static constexpr const char* rOffsetJsonKey = "rOffset";
    static constexpr const char* gOffsetJsonKey = "gOffset";
    static constexpr const char* bOffsetJsonKey = "bOffset";
};

#endif /* PROCESSING_LINEARRGBFUNCTION_H_ */
