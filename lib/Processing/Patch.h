#ifndef PROCESSING_PATCH_H_
#define PROCESSING_PATCH_H_

#include <string>
#include <mutex>

#include "IPatch.h"

class IProcessingBlockFactory;

/**
 * Class which represents a patch.
 *
 * This class does not represent a software patch (lines of code to apply to some other code).
 * Instead, it's more like an analog synthesizer patch. It contains the configuration of the processing chain,
 * so light is generated from MIDI data in a certain way.
 */
class Patch
    : public IPatch
{
public:
    /**
     * Constructor.
     *
     * @param[in]   processingBlockFactory  Reference to the processing block factory.
     */
    Patch(const IProcessingBlockFactory& processingBlockFactory);

    /**
     * Destructor.
     */
    virtual ~Patch();

    // Prevent implicit constructors and assignment operator.
    Patch() = delete;
    Patch(const Patch&) = delete;
    Patch& operator=(const Patch&) = delete;

    // IJsonConvertible implementation
    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& converted);

    // IPatch implementation
    virtual IProcessingChain& getProcessingChain() const;
    virtual void activate();
    virtual void deactivate();
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap);
    virtual bool hasBankAndProgram() const;
    virtual uint8_t getBank() const;
    virtual void setBank(uint8_t bank);
    virtual uint8_t getProgram() const;
    virtual void setProgram(uint8_t program);
    virtual void clearBankAndProgram();
    virtual std::string getName() const;
    virtual void setName(const std::string name);

protected:
    // IJsonConvertible implementation
    std::string getObjectType() const;

private:
    static constexpr const char* typeName                 = "Patch";
    static constexpr const char* hasBankAndProgramJsonKey = "hasBankAndProgram";
    static constexpr const char* bankJsonKey              = "bank";
    static constexpr const char* programJsonKey           = "program";
    static constexpr const char* nameJsonKey              = "name";
    static constexpr const char* processingChainJsonKey   = "processingChain";

    mutable std::mutex mutex;

    bool bankAndProgramSet = false;
    uint8_t bank = 0;
    uint8_t program = 0;
    std::string name = "Untitled Patch";
    IProcessingChain* processingChain;
    const IProcessingBlockFactory& processingBlockFactory;
};

#endif /* PROCESSING_PATCH_H_ */
