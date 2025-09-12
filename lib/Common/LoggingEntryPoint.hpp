#ifndef COMMON_LOGGINGENTRYPOINT_H_
#define COMMON_LOGGINGENTRYPOINT_H_

#include <cstdint>
#include <mutex>
#include <vector>

#include "LoggingDefinitions.hpp"

class LoggingTarget;
class MonotonicTime;

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
    static void subscribe(LoggingTarget& subscriber);

    /**
     * Unsubscribe from logging events.
     *
     * @param[in]   subscriber  Reference to the subscriber.
     */
    static void unsubscribe(LoggingTarget& subscriber);

    /**
     * Set the time provider.
     */
    static void setTime(const MonotonicTime* time);

    /**
     * Log a message.
     *
     * @param[in]   level       Log level.
     * @param[in]   component   Originating component.
     * @param[in]   fmt         Format string of the log message.
     * @param[in]   ...         Arguments to use for string formatting.
     */
    static void logMessage(logging::Level level, const char* component, const char* fmt, ...)
        __attribute__((format(printf, 3, 4)));

    /** Max log message size, excluding file, line and level information. */
    static constexpr unsigned int maxMessageSize = 2048;

  private:
    /** The list of subscribers. */
    static std::vector<LoggingTarget*> subscribers;

    /** Mutex to protect the list of subscribers. */
    static std::mutex mutex;

    static const MonotonicTime* monotonicTime;
};

#endif /* COMMON_LOGGINGENTRYPOINT_H_ */
