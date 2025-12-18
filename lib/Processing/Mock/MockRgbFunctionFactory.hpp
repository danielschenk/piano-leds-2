#ifndef PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_
#define PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_

#include <gmock/gmock.h>

#include "../RgbFunctionFactory.hpp"

namespace processing
{

class MockRgbFunctionFactory : public IRgbFunctionFactory
{
  public:
    MOCK_CONST_METHOD1(createRgbFunction, std::shared_ptr<RgbFunction>(const Json& converted));
};

}  // namespace processing

#endif /* PROCESSING_MOCK_MOCKRGBFUNCTIONFACTORY_H_ */
