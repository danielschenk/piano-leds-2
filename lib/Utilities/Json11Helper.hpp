#ifndef COMMON_UTILITIES_JSON11HELPER_H_
#define COMMON_UTILITIES_JSON11HELPER_H_

#include <json11.hpp>
#include <string>
// for convenience
using Json = json11::Json;

#include "Logging.hpp"

#define LOGGING_COMPONENT "Json11Helper"

/**
 * @brief Helper class to fetch JSON items with type checking.
 */
class Json11Helper
{
  public:
    /**
     * Constructor.
     *
     * @param[in]   user            Name of the user for logging errors.
     * @param[in]   json            JSON object to work with.
     * @param[in]   logMissingKeys  If missing keys should be logged.
     */
    Json11Helper(std::string user, const Json& json, bool logMissingKeys = true);
    Json11Helper(const char* user, const Json& json, bool logMissingKeys = true);

    virtual ~Json11Helper();

    template <typename T>
    bool getItemIfPresent(std::string key, T& target) const
    {
        if (!json[key].is_null())
        {
            return getItem(key, target);
        }
        else
        {
            if (logMissingKeys)
            {
                LOG_ERROR_PARAMS("%s: Missing JSON key '%s'", user.c_str(), key.c_str());
            }
            return false;
        }
    }

    Json11Helper() = delete;
    Json11Helper(const Json11Helper&) = delete;
    Json11Helper& operator=(const Json11Helper&) = delete;

  private:
    template <typename IntegerType>
    bool getInt(std::string key, IntegerType& target) const
    {
        const auto& item = json[key];

        if (item.is_number())
        {
            target = static_cast<IntegerType>(item.int_value());
            return true;
        }
        else
        {
            LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a number", user.c_str(),
                             key.c_str());
            return false;
        }
    }

    template <typename FloatType>
    bool getFloat(std::string key, FloatType& target) const
    {
        const auto& item = json[key];

        if (item.is_number())
        {
            target = static_cast<FloatType>(item.number_value());
            return true;
        }
        else
        {
            LOG_ERROR_PARAMS("%s: JSON value with key '%s' not a number", user.c_str(),
                             key.c_str());
            return false;
        }
    }

    bool getItem(std::string key, int& target) const;
    bool getItem(std::string key, uint8_t& target) const;
    bool getItem(std::string key, uint16_t& target) const;
    bool getItem(std::string key, float& target) const;
    bool getItem(std::string key, double& target) const;
    bool getItem(std::string key, bool& target) const;
    bool getItem(std::string key, std::string& target) const;
    bool getItem(std::string key, Json::object& target) const;
    bool getItem(std::string key, Json::array& target) const;

    std::string user;
    const Json& json;
    bool logMissingKeys;
};

#undef LOGGING_COMPONENT

#endif /* COMMON_UTILITIES_JSON11HELPER_H_ */
