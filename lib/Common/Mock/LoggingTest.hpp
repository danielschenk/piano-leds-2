#ifndef COMMON_MOCK_LOGGINGTEST_H_
#define COMMON_MOCK_LOGGINGTEST_H_

#include <gtest/gtest.h>

#include "../LoggingEntryPoint.hpp"
#include "MockLoggingTarget.hpp"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::AnyOf;

class LoggingTest
{
  public:
    LoggingTest() : mockLoggingTarget()
    {
        EXPECT_CALL(mockLoggingTarget,
                    logMessage(_, AnyOf(logging::Level::info, logging::Level::debug), _, _))
            .Times(AnyNumber());

        LoggingEntryPoint::subscribe(mockLoggingTarget);
    }

    virtual ~LoggingTest()
    {
        LoggingEntryPoint::unsubscribe(mockLoggingTarget);
    }

    MockLoggingTarget mockLoggingTarget;
};

#endif /* COMMON_MOCK_LOGGINGTEST_H_ */
