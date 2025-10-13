#ifndef PROCESSING_SINGLECOLORFILL_H_
#define PROCESSING_SINGLECOLORFILL_H_

#include <mutex>

#include "Parameter.hpp"
#include "ProcessingBlock.hpp"

class SingleColorFill : public ProcessingBlock
{
  public:
    SingleColorFill() = default;

    SingleColorFill(SingleColorFill&) = delete;
    SingleColorFill& operator=(SingleColorFill&) = delete;

    // ProcessingBlock implementation.
    void activate() override;
    void deactivate() override;
    void execute(processing::RgbStrip& strip,
                 const processing::NoteToLightMap& noteToLightMap) override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

    std::map<std::string, processing::Modulatable*> modulationSinks() const override;

    processing::Parameter<processing::RgbColor> color;

  protected:
    // ProcessingBlock implementation
    std::string getObjectType() const override;

  private:
    static constexpr const char* rJsonKey = "r";
    static constexpr const char* gJsonKey = "g";
    static constexpr const char* bJsonKey = "b";

    mutable std::mutex mutex;
};

#endif /* PROCESSING_SINGLECOLORFILL_H_ */
