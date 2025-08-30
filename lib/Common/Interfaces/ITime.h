#ifndef INTERFACES_ITIME_H_
#define INTERFACES_ITIME_H_

#include <stdint.h>

class ITime
{
public:
    /**
     * Get the monotonic time in milliseconds.
     */
    virtual uint32_t getMilliseconds() const = 0;

protected:
    virtual ~ITime() = default;
};

#endif /* INTERFACES_ITIME_H_ */
