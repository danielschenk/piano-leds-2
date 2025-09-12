#ifndef COMMON_LOGGINGENTRYPOINT_H_
#define COMMON_LOGGINGENTRYPOINT_H_

#include <cstdint>
#include <mutex>
#include <vector>

#include "LoggingDefinitions.h"

class ILoggingTarget;
class ITime;

/**
 * Static class which receives log events and distributes them to subscribers (logging
 * implementations).
 */
class LoggingEntryPoint
{
  public:
    // Prevent implicit constructor, copy constructor and assignment operator.
    LoggingEntryPoint() = delete;
    LoggingEntryPoint(const LoggingEntryPoint&) = delete;
    LoggingEntryPoint& operator=(const LoggingEntryPoint&) = delete;

    /**
     * Subscribe to logging events.
     *
     * @param[in]   subscriber  Reference to the subscriber.
     */
    static void subscribe(ILoggingTarget& subscriber);

    /**
     * Unsubscribe from logging events.
     *
     * @param[in]   subscriber  Reference to the subscriber.
     */
    static void unsubscribe(ILoggingTarget& subscriber);

    /**
     * Set the time provider.
     */
    static void setTime(const ITime* time);

    /**
     * Log a message.
     *
     * @param[in]   level       Log level.
     * @param[in]   component   Originating component.
     * @param[in]   fmt         Format string of the log message.
     * @param[in]   ...         Arguments to use for string formatting.
     */
    static void logMessage(Logging::TLogLevel level, const char* component, const char* fmt, ...)
        __attribute__((format(printf, 3, 4)));

    /** Max log message size, excluding file, line and level information. */
    static constexpr unsigned int maxMessageSize = 2048;

  private:
    /** The list of subscribers. */
    static std::vector<ILoggingTarget*> s_subscribers;

    /** Mutex to protect the list of subscribers. */
    static std::mutex s_mutex;

    static const ITime* s_time;
};

#endif /* COMMON_LOGGINGENTRYPOINT_H_ */
