#ifndef COMMON_MOCK_MOCKLOGGINGTARGET_H_
#define COMMON_MOCK_MOCKLOGGINGTARGET_H_

#include <gmock/gmock.h>

#include "LoggingTarget.hpp"

class MockLoggingTarget : public LoggingTarget
{
  public:
    // LoggingTarget implementation
    MOCK_METHOD4(logMessage, void(uint64_t time, logging::Level level, std::string component,
                                  std::string message));
};

#endif /* COMMON_MOCK_MOCKLOGGINGTARGET_H_ */
