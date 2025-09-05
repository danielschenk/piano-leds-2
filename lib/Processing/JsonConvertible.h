#ifndef PROCESSING_JSONCONVERTIBLE_H_
#define PROCESSING_JSONCONVERTIBLE_H_

#include <string>
#include <json11.hpp>
// for convenience
using Json = json11::Json;

class JsonConvertible
{
public:
    static constexpr const char* objectTypeKey = "objectType";

    virtual ~JsonConvertible() = default;

    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& json) = 0;

protected:
    virtual std::string getObjectType() const;
};


#endif /* PROCESSING_JSONCONVERTIBLE_H_ */
