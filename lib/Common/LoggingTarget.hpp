#ifndef COMMON_LOGGINGTARGET_HPP
#define COMMON_LOGGINGTARGET_HPP

#include <cstdint>
#include <string>

#include "LoggingDefinitions.hpp"

class LoggingTarget
{
  public:
    virtual void logMessage(uint64_t time, logging::Level level, std::string component,
                            std::string message) = 0;

  protected:
    virtual ~LoggingTarget() = default;
};

#endif /* COMMON_LOGGINGTARGET_HPP */
