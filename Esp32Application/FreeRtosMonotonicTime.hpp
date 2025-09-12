#ifndef ESP32APPLICATION_FREERTOSMONOTONICTIME_HPP
#define ESP32APPLICATION_FREERTOSMONOTONICTIME_HPP

#include "MonotonicTime.hpp"

class FreeRtosMonotonicTime : public MonotonicTime
{
  public:
    FreeRtosMonotonicTime() = default;
    ~FreeRtosMonotonicTime() override = default;

    // MonotonicTime implementation
    uint32_t getMilliseconds() const override;
};

#endif /* ESP32APPLICATION_FREERTOSMONOTONICTIME_HPP */
