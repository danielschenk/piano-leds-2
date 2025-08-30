#ifndef PROCESSING_INTERFACES_IJSONCONVERTIBLE_H_
#define PROCESSING_INTERFACES_IJSONCONVERTIBLE_H_

#include <string>
#include <json11.hpp>
// for convenience
using Json = json11::Json;

/**
 * Interface for objects which can be converted to/from JSON.
 */
class IJsonConvertible
{
public:
    static constexpr const char* objectTypeKey = "objectType";

    /**
     * Destructor.
     */
    virtual ~IJsonConvertible()
    {
    }

    /**
     * Convert object to JSON.
     *
     * @return The JSON object containing the persistent properties of the object.
     */
    virtual Json convertToJson() const = 0;

    /**
     * Convert object from JSON.
     *
     * @param[in]   converted   The JSON object containing the persistent properties of the object.
     */
    virtual void convertFromJson(const Json& converted) = 0;

protected:
    /**
     * Get the object type.
     *
     * @return The object type.
     */
    virtual std::string getObjectType() const = 0;
};


#endif /* PROCESSING_INTERFACES_IJSONCONVERTIBLE_H_ */
