#ifndef PROCESSING_SINGLECOLORFILL_H_
#define PROCESSING_SINGLECOLORFILL_H_

#include <mutex>

#include "ProcessingBlock.hpp"

class SingleColorFill : public ProcessingBlock
{
  public:
    SingleColorFill() = default;

    SingleColorFill(SingleColorFill&) = delete;
    SingleColorFill& operator=(SingleColorFill&) = delete;

    // ProcessingBlock implementation.
    virtual void activate();
    virtual void deactivate();
    virtual void execute(processing::RgbStrip& strip,
                         const processing::NoteToLightMap& noteToLightMap);
    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& converted);

    processing::RgbColor getColor() const;
    void setColor(processing::RgbColor color);

  protected:
    // ProcessingBlock implementation
    virtual std::string getObjectType() const;

  private:
    static constexpr const char* rJsonKey = "r";
    static constexpr const char* gJsonKey = "g";
    static constexpr const char* bJsonKey = "b";

    mutable std::mutex mutex;

    /** Output color. */
    processing::RgbColor color;
};

#endif /* PROCESSING_SINGLECOLORFILL_H_ */
