#ifndef PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_
#define PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_

#include <json11.hpp>
#include <memory>

using Json = json11::Json;

class IRgbFunction;

class IRgbFunctionFactory
{
public:
    virtual ~IRgbFunctionFactory() = default;
    virtual std::shared_ptr<IRgbFunction> createRgbFunction(const Json& converted) const = 0;
};



#endif /* PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_ */
