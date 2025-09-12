#include "LinearRgbFunction.h"

#include "Json11Helper.h"

LinearRgbFunction::LinearRgbFunction(const Processing::TRgb& color)
{
    redConstants = {2.0f * color.r / 255.0f, 0};
    greenConstants = {2.0f * color.g / 255.0f, 0};
    blueConstants = {2.0f * color.b / 255.0f, 0};
}

Processing::TRgb LinearRgbFunction::calculate(const Processing::TNoteState& noteState,
                                              Processing::TTime currentTime) const
{
    Processing::TRgb output;

    if (noteState.sounding)
    {
        output = Processing::rgbFromFloat(
            redConstants.factor * noteState.pressDownVelocity + redConstants.offset,
            greenConstants.factor * noteState.pressDownVelocity + greenConstants.offset,
            blueConstants.factor * noteState.pressDownVelocity + blueConstants.offset);

        if (noteState.pressDownColor != Processing::ColorValue::off)
            output *= noteState.pressDownColor;
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
    json[JsonConvertible::objectTypeKey] = getObjectType();
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
