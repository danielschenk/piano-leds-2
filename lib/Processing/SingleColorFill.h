#ifndef PROCESSING_SINGLECOLORFILL_H_
#define PROCESSING_SINGLECOLORFILL_H_

#include "ProcessingBlock.h"
#include <mutex>

class SingleColorFill
    : public ProcessingBlock
{
public:
    SingleColorFill() = default;

    SingleColorFill(SingleColorFill&) = delete;
    SingleColorFill& operator=(SingleColorFill&) = delete;

    // ProcessingBlock implementation.
    virtual void activate();
    virtual void deactivate();
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap);
    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& converted);

    Processing::TRgb getColor() const;
    void setColor(Processing::TRgb color);

protected:
    // ProcessingBlock implementation
    virtual std::string getObjectType() const;

private:
    static constexpr const char* rJsonKey = "r";
    static constexpr const char* gJsonKey = "g";
    static constexpr const char* bJsonKey = "b";

    mutable std::mutex mutex;

    /** Output color. */
    Processing::TRgb color;
};

#endif /* PROCESSING_SINGLECOLORFILL_H_ */
