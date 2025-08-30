#include "EqualRangeRgbSource.h"
#include "Json11Helper.h"

void EqualRangeRgbSource::activate()
{
}

void EqualRangeRgbSource::deactivate()
{
}

void EqualRangeRgbSource::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    std::lock_guard<std::mutex> lock(mutex);

    for(auto& it : strip)
    {
        it.r = color.r;
        it.g = color.g;
        it.b = color.b;
    }
}

Processing::TRgb EqualRangeRgbSource::getColor() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return color;
}

void EqualRangeRgbSource::setColor(Processing::TRgb color)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->color = color;
}

Json EqualRangeRgbSource::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object json;
    json[IJsonConvertible::objectTypeKey] = getObjectType();
    json[rJsonKey] = color.r;
    json[gJsonKey] = color.g;
    json[bJsonKey] = color.b;

    return Json(json);
}

void EqualRangeRgbSource::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(rJsonKey, color.r);
    helper.getItemIfPresent(gJsonKey, color.g);
    helper.getItemIfPresent(bJsonKey, color.b);
}

std::string EqualRangeRgbSource::getObjectType() const
{
     return IProcessingBlock::typeNameEqualRangeRgbSource;
}
