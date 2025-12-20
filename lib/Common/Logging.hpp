#ifndef COMMON_LOGGING_H_
#define COMMON_LOGGING_H_

#include "LoggingEntryPoint.hpp"

#if ENABLE_LOG_ERROR
#define LOG_ERROR(fmt, ...) \
    LoggingEntryPoint::logMessage(logging::Level::error, LOGGING_COMPONENT, (fmt))
#define LOG_ERROR_PARAMS(fmt, ...) \
    LoggingEntryPoint::logMessage(logging::Level::error, LOGGING_COMPONENT, (fmt), __VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)
#define LOG_ERROR_PARAMS(fmt, ...)
#endif

#if ENABLE_LOG_WARNING
#define LOG_WARNING(fmt, ...) \
    LoggingEntryPoint::logMessage(logging::Level::warning, LOGGING_COMPONENT, (fmt))
#define LOG_WARNING_PARAMS(fmt, ...) \
    LoggingEntryPoint::logMessage(logging::Level::warning, LOGGING_COMPONENT, (fmt), __VA_ARGS__)
#else
#define LOG_WARNING(fmt, ...)
#define LOG_WARNING_PARAMS(fmt, ...)
#endif

#if ENABLE_LOG_INFO
#define LOG_INFO(fmt, ...) \
    LoggingEntryPoint::logMessage(logging::Level::info, LOGGING_COMPONENT, (fmt))
#define LOG_INFO_PARAMS(fmt, ...) \
    LoggingEntryPoint::logMessage(logging::Level::info, LOGGING_COMPONENT, (fmt), __VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)
#define LOG_INFO_PARAMS(fmt, ...)
#endif

#if ENABLE_LOG_DEBUG
#define LOG_DEBUG(fmt, ...) \
    LoggingEntryPoint::logMessage(logging::Level::debug, LOGGING_COMPONENT, (fmt))
#define LOG_DEBUG_PARAMS(fmt, ...) \
    LoggingEntryPoint::logMessage(logging::Level::debug, LOGGING_COMPONENT, (fmt), __VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#define LOG_DEBUG_PARAMS(fmt, ...)
#endif

#endif /* COMMON_LOGGING_H_ */
