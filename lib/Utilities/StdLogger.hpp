#ifndef COMMON_UTILITIES_STDLOGGER_H_
#define COMMON_UTILITIES_STDLOGGER_H_

#include "LoggingEntryPoint.hpp"
#include "LoggingTarget.hpp"

/**
 * Logging target which logs to std::cout
 */
class StdLogger : public LoggingTarget
{
  public:
    /**
     * Constructor.
     */
    StdLogger();

    /**
     * Destructor.
     */
    virtual ~StdLogger();

    // LoggingTarget implementation
    virtual void logMessage(uint64_t time, logging::Level level, std::string component,
                            std::string message);
};

#endif /* COMMON_UTILITIES_STDLOGGER_H_ */
