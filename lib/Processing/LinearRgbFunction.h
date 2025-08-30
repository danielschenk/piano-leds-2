/**
 * @file
 *
 * MIT License
 *
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @brief Function which describes a time invariant linear relation between note state and RGB output.
 */

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
    /**
     * Constructor.
     */
    LinearRgbFunction() = default;

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
