#include "JsonConvertible.hpp"
#include "ProcessingTypes.hpp"

// Technically this is not needed. But it's nice if you want to do:
// getProcessingChain().someMethod();
#include "IProcessingChain.hpp"
#include "ProcessingBlock.hpp"

class IPatch : public JsonConvertible
{
  public:
    virtual ~IPatch() = default;

    virtual IProcessingChain& getProcessingChain() const = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void execute(processing::RgbStrip& strip, const ProcessingBlock::Input& input) = 0;

    virtual bool hasBankAndProgram() const = 0;
    virtual uint8_t getBank() const = 0;
    virtual void setBank(uint8_t bank) = 0;
    virtual uint8_t getProgram() const = 0;
    virtual void setProgram(uint8_t program) = 0;
    virtual void clearBankAndProgram() = 0;
    virtual std::string getName() const = 0;
    virtual void setName(std::string name) = 0;
};
