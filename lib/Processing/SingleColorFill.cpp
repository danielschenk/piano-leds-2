#include "SingleColorFill.hpp"

#include "Json11Helper.hpp"

void SingleColorFill::activate() {}

void SingleColorFill::deactivate() {}

void SingleColorFill::execute(processing::RgbStrip& strip,
                              const processing::NoteToLightMap& noteToLightMap)
{
    std::lock_guard<std::mutex> lock(mutex);

    for (auto& it : strip)
    {
        it.r = properties.color->r;
        it.g = properties.color->g;
        it.b = properties.color->b;
    }
}

Json SingleColorFill::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object json;
    json[JsonConvertible::objectTypeKey] = getObjectType();
    json[rJsonKey] = properties.color->r;
    json[gJsonKey] = properties.color->g;
    json[bJsonKey] = properties.color->b;

    return Json(json);
}

void SingleColorFill::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(rJsonKey, properties.color->r);
    helper.getItemIfPresent(gJsonKey, properties.color->g);
    helper.getItemIfPresent(bJsonKey, properties.color->b);
}

std::string SingleColorFill::getObjectType() const
{
    return ProcessingBlock::typeNameSingleColorFill;
}
