#ifndef PROCESSING_MOCK_MOCKRGBFUNCTION_H_
#define PROCESSING_MOCK_MOCKRGBFUNCTION_H_

#include <gmock/gmock.h>

#include "../Interfaces/IRgbFunction.h"

class MockRgbFunction : public IRgbFunction
{
  public:
    MOCK_CONST_METHOD2(calculate, Processing::TRgb(const Processing::TNoteState& noteState,
                                                   Processing::TTime currentTime));
    MOCK_CONST_METHOD0(convertToJson, Json());
    MOCK_METHOD1(convertFromJson, void(const Json& converted));

  protected:
    MOCK_CONST_METHOD0(getObjectType, std::string());
};

#endif /* PROCESSING_MOCK_MOCKRGBFUNCTION_H_ */
