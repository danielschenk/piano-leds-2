#ifndef COMMON_UTILITIES_STDLOGGER_H_
#define COMMON_UTILITIES_STDLOGGER_H_

#include "ILoggingTarget.h"
#include "LoggingEntryPoint.h"

/**
 * Logging target which logs to std::cout
 */
class StdLogger : public ILoggingTarget
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

    // ILoggingTarget implementation
    virtual void logMessage(uint64_t time, Logging::TLogLevel level, std::string component,
                            std::string message);
};

#endif /* COMMON_UTILITIES_STDLOGGER_H_ */
