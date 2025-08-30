#ifndef COMMON_INTERFACES_ILOGGINGTARGET_H_
#define COMMON_INTERFACES_ILOGGINGTARGET_H_

#include <string>
#include <cstdint>

#include "LoggingDefinitions.h"

/**
 * Interface for logging targets.
 */
class ILoggingTarget
{
public:
    /**
     * Log a message.
     *
     * @param[in]   time        Timestamp of the log call.
     * @param[in]   level       Log level.
     * @param[in]   component   Originating component.
     * @param[in]   message     The log message.
     */
    virtual void logMessage(uint64_t time, Logging::TLogLevel level, std::string component, std::string message) = 0;

protected:
    /**
     * Destructor.
     */
    virtual ~ILoggingTarget() = default;
};



#endif /* COMMON_INTERFACES_ILOGGINGTARGET_H_ */
