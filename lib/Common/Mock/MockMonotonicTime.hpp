#ifndef COMMON_MOCKMONOTONICTIME_H_
#define COMMON_MOCKMONOTONICTIME_H_

#include <gmock/gmock.h>

#include "MonotonicTime.hpp"

class MockMonotonicTime : public MonotonicTime
{
  public:
    MOCK_CONST_METHOD0(getMilliseconds, uint32_t());
};

#endif /* COMMON_MOCKMONOTONICTIME_H_ */
