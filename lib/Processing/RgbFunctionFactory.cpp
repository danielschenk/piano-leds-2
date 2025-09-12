#include "RgbFunctionFactory.hpp"

#include "IRgbFunction.hpp"
#include "Json11Helper.hpp"
#include "JsonConvertible.hpp"
#include "LinearRgbFunction.hpp"
#include "PianoDecayRgbFunction.hpp"

std::shared_ptr<IRgbFunction> RgbFunctionFactory::createRgbFunction(const Json& converted) const
{
    std::shared_ptr<IRgbFunction> rgbFunction;

    Json11Helper helper(__PRETTY_FUNCTION__, converted);

    std::string objectType;
    if (helper.getItemIfPresent(JsonConvertible::objectTypeKey, objectType))
    {
        if (objectType == IRgbFunction::jsonTypeNameLinearRgbFunction)
        {
            rgbFunction = std::make_shared<LinearRgbFunction>();
        }
        else if (objectType == IRgbFunction::jsonTypeNamePianoDecayRgbFunction)
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
