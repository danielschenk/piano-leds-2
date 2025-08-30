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
 */

#include "LinearRgbFunction.h"
#include "Json11Helper.h"

Processing::TRgb LinearRgbFunction::calculate(const Processing::TNoteState& noteState, Processing::TTime currentTime) const
{
    Processing::TRgb output;

    if(noteState.sounding)
    {
        output = Processing::rgbFromFloat(
            redConstants.factor * noteState.pressDownVelocity + redConstants.offset,
            greenConstants.factor * noteState.pressDownVelocity + greenConstants.offset,
            blueConstants.factor * noteState.pressDownVelocity + blueConstants.offset
        );
    }

    return output;
}

void LinearRgbFunction::setRedConstants(Processing::TLinearConstants redConstants)
{
    this->redConstants = redConstants;
}

void LinearRgbFunction::setGreenConstants(Processing::TLinearConstants greenConstants)
{
    this->greenConstants = greenConstants;
}

void LinearRgbFunction::setBlueConstants(Processing::TLinearConstants blueConstants)
{
    this->blueConstants = blueConstants;
}

Processing::TLinearConstants LinearRgbFunction::getRedConstants() const
{
    return redConstants;
}

Processing::TLinearConstants LinearRgbFunction::getGreenConstants() const
{
    return greenConstants;
}

Processing::TLinearConstants LinearRgbFunction::getBlueConstants() const
{
    return blueConstants;
}

Json LinearRgbFunction::convertToJson() const
{
    Json::object json;
    json[IJsonConvertible::objectTypeKey] = getObjectType();
    json[rFactorJsonKey] = redConstants.factor;
    json[gFactorJsonKey] = greenConstants.factor;
    json[bFactorJsonKey] = blueConstants.factor;
    json[rOffsetJsonKey] = redConstants.offset;
    json[gOffsetJsonKey] = greenConstants.offset;
    json[bOffsetJsonKey] = blueConstants.offset;

    return Json(json);
}

void LinearRgbFunction::convertFromJson(const Json& converted)
{
    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(rFactorJsonKey, redConstants.factor);
    helper.getItemIfPresent(rOffsetJsonKey, redConstants.offset);
    helper.getItemIfPresent(gFactorJsonKey, greenConstants.factor);
    helper.getItemIfPresent(gOffsetJsonKey, greenConstants.offset);
    helper.getItemIfPresent(bFactorJsonKey, blueConstants.factor);
    helper.getItemIfPresent(bOffsetJsonKey, blueConstants.offset);
}

std::string LinearRgbFunction::getObjectType() const
{
    return IRgbFunction::jsonTypeNameLinearRgbFunction;
}
