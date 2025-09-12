#ifndef ESP32APPLICATION_FREERTOSTIME_H_
#define ESP32APPLICATION_FREERTOSTIME_H_

#include "ITime.h"

/**
 * Time implementation around FreeRTOS.
 */
class FreeRtosTime : public ITime
{
  public:
    FreeRtosTime() = default;
    ~FreeRtosTime() override = default;

    // ITime implementation
    uint32_t getMilliseconds() const override;
};

#endif /* ESP32APPLICATION_FREERTOSTIME_H_ */
