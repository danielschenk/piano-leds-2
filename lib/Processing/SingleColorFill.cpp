#include "SingleColorFill.hpp"

#include "Json11Helper.hpp"

void SingleColorFill::activate() {}

void SingleColorFill::deactivate() {}

void SingleColorFill::execute(processing::RgbStrip& strip,
                              const processing::TNoteToLightMap& noteToLightMap)
{
    std::lock_guard<std::mutex> lock(mutex);

    for (auto& it : strip)
    {
        it.r = color.r;
        it.g = color.g;
        it.b = color.b;
    }
}

processing::RgbColor SingleColorFill::getColor() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return color;
}

void SingleColorFill::setColor(processing::RgbColor color)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->color = color;
}

Json SingleColorFill::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object json;
    json[JsonConvertible::objectTypeKey] = getObjectType();
    json[rJsonKey] = color.r;
    json[gJsonKey] = color.g;
    json[bJsonKey] = color.b;

    return Json(json);
}

void SingleColorFill::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(rJsonKey, color.r);
    helper.getItemIfPresent(gJsonKey, color.g);
    helper.getItemIfPresent(bJsonKey, color.b);
}

std::string SingleColorFill::getObjectType() const
{
    return ProcessingBlock::typeNameSingleColorFill;
}
