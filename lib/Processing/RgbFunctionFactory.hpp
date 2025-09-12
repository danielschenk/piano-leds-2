#ifndef PROCESSING_RGBFUNCTIONFACTORY_H_
#define PROCESSING_RGBFUNCTIONFACTORY_H_

#include <memory>

#include "IRgbFunctionFactory.hpp"

/**
 * Factory for RGB functions.
 */
class RgbFunctionFactory : public IRgbFunctionFactory
{
  public:
    RgbFunctionFactory() = default;

    RgbFunctionFactory(const RgbFunctionFactory&) = delete;
    RgbFunctionFactory& operator=(const RgbFunctionFactory&) = delete;

    virtual ~RgbFunctionFactory() = default;

    // IRgbFunctionFactory implementation
    virtual std::shared_ptr<IRgbFunction> createRgbFunction(const Json& converted) const;
};

#endif /* PROCESSING_RGBFUNCTIONFACTORY_H_ */
