#ifndef COMMON_MOCK_LOGGINGTEST_H_
#define COMMON_MOCK_LOGGINGTEST_H_

#include <gtest/gtest.h>

#include "MockLoggingTarget.h"
#include "../LoggingEntryPoint.h"

using ::testing::AnyOf;
using ::testing::_;
using ::testing::AnyNumber;

class LoggingTest
{
public:
    LoggingTest()
        : mockLoggingTarget()
    {
        // Info and debug logs are OK
        EXPECT_CALL(mockLoggingTarget, logMessage(_, AnyOf(Logging::LogLevel_Info, Logging::LogLevel_Debug), _, _))
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
