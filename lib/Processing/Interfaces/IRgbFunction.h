#ifndef PROCESSING_IRGBFUNCTION_H_
#define PROCESSING_IRGBFUNCTION_H_

#include "JsonConvertible.h"
#include "ProcessingTypes.h"

/**
 * Interface for RGB functions.
 */
class IRgbFunction
    : public JsonConvertible
{
public:
    static constexpr const char* jsonTypeNameLinearRgbFunction = "LinearRgbFunction";
    static constexpr const char* jsonTypeNamePianoDecayRgbFunction = "PianoDecayRgbFunction";

    /**
     * Calculate the output color based on the given note state and current time.
     *
     * @param[in]   noteState   The note state.
     * @param[in]   currentTime The current time.
     *
     * @return  The output color.
     */
    virtual Processing::TRgb calculate(const Processing::TNoteState& noteState, Processing::TTime currentTime) const = 0;
};

#endif /* PROCESSING_IRGBFUNCTION_H_ */
