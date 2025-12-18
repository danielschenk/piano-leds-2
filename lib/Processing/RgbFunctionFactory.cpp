#include "RgbFunctionFactory.hpp"

#include "Json11Helper.hpp"
#include "JsonConvertible.hpp"
#include "LinearRgbFunction.hpp"
#include "PianoDecayRgbFunction.hpp"
#include "RgbFunction.hpp"

std::shared_ptr<RgbFunction> RgbFunctionFactory::createRgbFunction(const Json& converted) const
{
    std::shared_ptr<RgbFunction> rgbFunction;

    Json11Helper helper(__PRETTY_FUNCTION__, converted);

    std::string objectType;
    if (helper.getItemIfPresent(JsonConvertible::objectTypeKey, objectType))
    {
        if (objectType == RgbFunction::jsonTypeNameLinearRgbFunction)
        {
            rgbFunction = std::make_shared<LinearRgbFunction>();
        }
        else if (objectType == RgbFunction::jsonTypeNamePianoDecayRgbFunction)
        {
            rgbFunction = std::make_shared<PianoDecayRgbFunction>();
        }

        if (rgbFunction != nullptr)
        {
            rgbFunction->convertFromJson(converted);
        }
    }

    return rgbFunction;
}
