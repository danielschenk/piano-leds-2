#include "RgbFunctionFactory.h"
#include "PianoDecayRgbFunction.h"
#include "LinearRgbFunction.h"
#include "JsonConvertible.h"
#include "IRgbFunction.h"

#include "Json11Helper.h"

std::shared_ptr<IRgbFunction> RgbFunctionFactory::createRgbFunction(const Json& converted) const
{
    std::shared_ptr<IRgbFunction> rgbFunction;

    Json11Helper helper(__PRETTY_FUNCTION__, converted);

    std::string objectType;
    if(helper.getItemIfPresent(JsonConvertible::objectTypeKey, objectType))
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
