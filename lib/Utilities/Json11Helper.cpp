#include "Json11Helper.h"

#define LOGGING_COMPONENT "Json11Helper"

Json11Helper::Json11Helper(std::string user, const Json& json, bool logMissingKeys)
    : user(user)
    , json(json)
    , logMissingKeys(logMissingKeys)
{
}

Json11Helper::Json11Helper(const char* user, const Json& json, bool logMissingKeys)
    : user(user)
    , json(json)
    , logMissingKeys(logMissingKeys)
{
    if(!json.is_object())
    {
        LOG_ERROR_PARAMS("%s: Passed object not a JSON object", this->user.c_str());
    }
}

Json11Helper::~Json11Helper()
{
}

bool Json11Helper::getItem(std::string key, int& target) const
{
    return getInt(key, target);
}

bool Json11Helper::getItem(std::string key, uint8_t& target) const
{
    return getInt(key, target);
}

bool Json11Helper::getItem(std::string key, uint16_t& target) const
{
    return getInt(key, target);
}

bool Json11Helper::getItem(std::string key, float& target) const
{
    return getFloat(key, target);
}

bool Json11Helper::getItem(std::string key, double& target) const
{
    return getFloat(key, target);
}

bool Json11Helper::getItem(std::string key, bool& target) const
{
    const auto& item = json[key];

    if(item.is_bool())
    {
        target = item.bool_value();
        return true;
    }
    else
    {
        LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a boolean", user.c_str(), key.c_str());
        return false;
    }
}

bool Json11Helper::getItem(std::string key, std::string& target) const
{
    const auto& item = json[key];

    if(item.is_string())
    {
        target = item.string_value();
        return true;
    }
    else
    {
        LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a string", user.c_str(), key.c_str());
        return false;
    }
}

bool Json11Helper::getItem(std::string key, Json::object& target) const
{
    const auto& item = json[key];

    if(item.is_object())
    {
        target = item.object_items();
        return true;
    }
    else
    {
        LOG_ERROR_PARAMS("%s: JSON value with key '%s' not an object", user.c_str(), key.c_str());
        return false;
    }
}

bool Json11Helper::getItem(std::string key, Json::array& target) const
{
    const auto& item = json[key];

    if(item.is_array())
    {
        target = item.array_items();
        return true;
    }
    else
    {
        LOG_ERROR_PARAMS("%s: JSON value with key '%s' not an array", user.c_str(), key.c_str());
        return false;
    }
}
