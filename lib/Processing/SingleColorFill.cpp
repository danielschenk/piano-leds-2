#include "SingleColorFill.hpp"

#include "Json11Helper.hpp"

SingleColorFill::SingleColorFill(const processing::RgbColor& color) : color(color) {}

void SingleColorFill::execute(processing::RgbStrip& strip, const Input& input)
{
    for (auto& it : strip)
    {
        it.r = color.r;
        it.g = color.g;
        it.b = color.b;
    }
}

Json SingleColorFill::convertToJson() const
{
    Json::object json;
    json[JsonConvertible::objectTypeKey] = getObjectType();
    json[rJsonKey] = color.r;
    json[gJsonKey] = color.g;
    json[bJsonKey] = color.b;

    return Json(json);
}

void SingleColorFill::convertFromJson(const Json& converted)
{
    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(rJsonKey, color.r);
    helper.getItemIfPresent(gJsonKey, color.g);
    helper.getItemIfPresent(bJsonKey, color.b);
}

std::string SingleColorFill::getObjectType() const
{
    return ProcessingBlock::typeNameSingleColorFill;
}
