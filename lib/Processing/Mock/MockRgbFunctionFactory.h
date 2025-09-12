#ifndef PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_
#define PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_

#include <gmock/gmock.h>

#include "../Interfaces/IRgbFunctionFactory.h"

class MockRgbFunctionFactory : public IRgbFunctionFactory
{
  public:
    MOCK_CONST_METHOD1(createRgbFunction, std::shared_ptr<IRgbFunction>(const Json& converted));
};

#endif /* PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_ */
