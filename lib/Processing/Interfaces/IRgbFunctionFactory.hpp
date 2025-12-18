#ifndef PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_
#define PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_

#include <json11.hpp>
#include <memory>

using Json = json11::Json;

namespace processing
{

class RgbFunction;

class IRgbFunctionFactory
{
  public:
    virtual ~IRgbFunctionFactory() = default;
    virtual std::shared_ptr<RgbFunction> createRgbFunction(const Json& converted) const = 0;
};

}  // namespace processing

#endif /* PROCESSING_INTERFACES_IRGBFUNCTIONFACTORY_H_ */
