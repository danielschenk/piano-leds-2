#ifndef MOCKTIME_H_
#define MOCKTIME_H_

#include <gmock/gmock.h>
#include "ITime.h"

class MockTime : public ITime
{
public:
    MOCK_CONST_METHOD0(getMilliseconds, uint32_t());
};


#endif /* MOCKTIME_H_ */
