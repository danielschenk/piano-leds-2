#ifndef COMMON_MONOTONICTIME_HPP
#define COMMON_MONOTONICTIME_HPP

#include <stdint.h>

class MonotonicTime
{
  public:
    virtual uint32_t getMilliseconds() const = 0;

  protected:
    virtual ~MonotonicTime() = default;
};

#endif /* COMMON_MONOTONICTIME_HPP */
