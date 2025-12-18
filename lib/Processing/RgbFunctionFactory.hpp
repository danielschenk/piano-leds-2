#ifndef PROCESSING_RGBFUNCTIONFACTORY_H_
#define PROCESSING_RGBFUNCTIONFACTORY_H_

#include <memory>

#include "IRgbFunctionFactory.hpp"

namespace processing
{

class RgbFunctionFactory : public IRgbFunctionFactory
{
  public:
    RgbFunctionFactory() = default;

    RgbFunctionFactory(const RgbFunctionFactory&) = delete;
    RgbFunctionFactory& operator=(const RgbFunctionFactory&) = delete;

    // IRgbFunctionFactory implementation
    std::shared_ptr<RgbFunction> createRgbFunction(const Json& converted) const override;
};

}  // namespace processing

#endif /* PROCESSING_RGBFUNCTIONFACTORY_H_ */
