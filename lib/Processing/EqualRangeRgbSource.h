#ifndef PROCESSING_EQUALRANGERGBSOURCE_H_
#define PROCESSING_EQUALRANGERGBSOURCE_H_

#include "IProcessingBlock.h"
#include <mutex>

/**
 * RGB source which generates an equal range of colors.
 */
class EqualRangeRgbSource
    : public IProcessingBlock
{
public:
    EqualRangeRgbSource() = default;

    EqualRangeRgbSource(EqualRangeRgbSource&) = delete;
    EqualRangeRgbSource& operator=(EqualRangeRgbSource&) = delete;

    // IProcessingBlock implementation.
    virtual void activate();
    virtual void deactivate();
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap);
    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& converted);

    /**
     * Get color.
     */
    Processing::TRgb getColor() const;

    /**
     * Set color.
     */
    void setColor(Processing::TRgb color);

protected:
    // IProcessingBlock implementation
    virtual std::string getObjectType() const;

private:
    static constexpr const char* rJsonKey = "r";
    static constexpr const char* gJsonKey = "g";
    static constexpr const char* bJsonKey = "b";

    mutable std::mutex mutex;

    /** Output color. */
    Processing::TRgb color;
};

#endif /* PROCESSING_EQUALRANGERGBSOURCE_H_ */
