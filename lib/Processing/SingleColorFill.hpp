#ifndef PROCESSING_SINGLECOLORFILL_H_
#define PROCESSING_SINGLECOLORFILL_H_

#include "ProcessingBlock.hpp"

class SingleColorFill : public ProcessingBlock
{
  public:
    SingleColorFill() = default;

    SingleColorFill(SingleColorFill&) = delete;
    SingleColorFill& operator=(SingleColorFill&) = delete;

    // ProcessingBlock implementation.
    void execute(processing::RgbStrip& strip,
                 const processing::NoteToLightMap& noteToLightMap) override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

    processing::RgbColor color;

  protected:
    // ProcessingBlock implementation
    std::string getObjectType() const override;

  private:
    static constexpr const char* rJsonKey = "r";
    static constexpr const char* gJsonKey = "g";
    static constexpr const char* bJsonKey = "b";
};

#endif /* PROCESSING_SINGLECOLORFILL_H_ */
