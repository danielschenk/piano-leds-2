#ifndef PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_
#define PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_

#include <gmock/gmock.h>

#include "../RgbFunctionFactory.hpp"

class MockRgbFunctionFactory : public IRgbFunctionFactory
{
  public:
    MOCK_CONST_METHOD1(createRgbFunction, std::shared_ptr<RgbFunction>(const Json& converted));
};

#endif /* PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_ */
