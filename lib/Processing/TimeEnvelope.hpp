#ifndef PROCESSING_TIMEENVELOPE_HPP
#define PROCESSING_TIMEENVELOPE_HPP

#include <cstdint>

namespace processing
{

struct TimeEnvelope
{
    uint32_t attackMs{1000};
    uint32_t releaseMs{1000};

    float calculateProgress(uint32_t now, uint32_t startTime) const;
    uint32_t duration() const;
    bool completed(uint32_t now, uint32_t startTime) const;
};

}  // namespace processing

#endif
