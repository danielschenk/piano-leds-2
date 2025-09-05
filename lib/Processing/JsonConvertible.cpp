#include "JsonConvertible.h"

Json JsonConvertible::convertToJson() const
{
    return Json();
}

void JsonConvertible::convertFromJson(const Json& json)
{
}

std::string JsonConvertible::getObjectType() const
{
    return "undefined";
}
