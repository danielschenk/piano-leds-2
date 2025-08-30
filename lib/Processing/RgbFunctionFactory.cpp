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

#include "RgbFunctionFactory.h"
#include "PianoDecayRgbFunction.h"
#include "LinearRgbFunction.h"
#include "IJsonConvertible.h"
#include "IRgbFunction.h"

#include "Json11Helper.h"

std::shared_ptr<IRgbFunction> RgbFunctionFactory::createRgbFunction(const Json& converted) const
{
    std::shared_ptr<IRgbFunction> rgbFunction;

    Json11Helper helper(__PRETTY_FUNCTION__, converted);

    std::string objectType;
    if(helper.getItemIfPresent(IJsonConvertible::objectTypeKey, objectType))
    {
        if(objectType == IRgbFunction::jsonTypeNameLinearRgbFunction)
        {
            rgbFunction = std::make_shared<LinearRgbFunction>();
        }
        else if(objectType == IRgbFunction::jsonTypeNamePianoDecayRgbFunction)
        {
            rgbFunction = std::make_shared<PianoDecayRgbFunction>();
        }

        if(rgbFunction != nullptr)
        {
            rgbFunction->convertFromJson(converted);
        }
    }

    return rgbFunction;
}
