#include "LinearRgbFunction.hpp"

#include "Json11Helper.hpp"

namespace processing
{

LinearRgbFunction::LinearRgbFunction(const processing::RgbColor& color)
{
    redConstants = {2.0f * color.r / 255.0f, 0};
    greenConstants = {2.0f * color.g / 255.0f, 0};
    blueConstants = {2.0f * color.b / 255.0f, 0};
}

processing::RgbColor LinearRgbFunction::calculate(const processing::NoteState& noteState,
                                                  processing::Timestamp currentTime) const
{
    processing::RgbColor output;

    if (noteState.sounding)
    {
        output = processing::RgbColor(
            redConstants.factor * noteState.pressDownVelocity + redConstants.offset,
            greenConstants.factor * noteState.pressDownVelocity + greenConstants.offset,
            blueConstants.factor * noteState.pressDownVelocity + blueConstants.offset);

        if (noteState.pressDownColor != processing::color_constants::off)
            output *= noteState.pressDownColor;
    }

    return output;
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
    return RgbFunction::jsonTypeNameLinearRgbFunction;
}

}  // namespace processing
