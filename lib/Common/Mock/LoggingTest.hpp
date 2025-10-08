#ifndef COMMON_MOCK_LOGGINGTEST_H_
#define COMMON_MOCK_LOGGINGTEST_H_

#include <gtest/gtest.h>

#include "../LoggingEntryPoint.hpp"
#include "MockLoggingTarget.hpp"
#include "MockMonotonicTime.hpp"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::AnyOf;
using ::testing::NiceMock;
using ::testing::Return;

class LoggingTest
{
  public:
    LoggingTest() : mockLoggingTarget()
    {
        ON_CALL(mockTime, getMilliseconds()).WillByDefault(Return(0));
        EXPECT_CALL(mockLoggingTarget,
                    logMessage(_, AnyOf(logging::Level::info, logging::Level::debug), _, _))
            .Times(AnyNumber());

        LoggingEntryPoint::setTime(&mockTime);
        LoggingEntryPoint::subscribe(mockLoggingTarget);
    }

    virtual ~LoggingTest()
    {
        LoggingEntryPoint::unsubscribe(mockLoggingTarget);
    }

    NiceMock<MockMonotonicTime> mockTime;
    MockLoggingTarget mockLoggingTarget;
};

#endif /* COMMON_MOCK_LOGGINGTEST_H_ */
