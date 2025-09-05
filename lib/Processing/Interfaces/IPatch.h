#include "JsonConvertible.h"
#include "ProcessingTypes.h"

// Technically this is not needed. But it's nice if you want to do:
// getProcessingChain().someMethod();
#include "IProcessingChain.h"

class IPatch
    : public JsonConvertible
{
public:
    virtual ~IPatch() = default;

    /**
     * Get the processing chain.
     */
    virtual IProcessingChain& getProcessingChain() const = 0;

    /**
     * Activate this patch.
     *
     * @post The patch responds to events.
     */
    virtual void activate() = 0;

    /**
     * Deactivate this patch.
     *
     * @post The patch ignores events.
     * @post The patch is in a clean state (as if no events have ever been received).
     */
    virtual void deactivate() = 0;

    /**
     * Execute this patch on the given strip.
     *
     * @param   [in/out]    strip           The strip to operate on.
     * @param   [in/out]    noteToLightMap  To map from note number to light number.
     */
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap) = 0;

    /**
     * Check if the patch has a valid bank and program number.
     *
     * @return  True if the patch has a valid bank and program number.
     */
    virtual bool hasBankAndProgram() const = 0;

    /**
     * Get the bank number.
     */
    virtual uint8_t getBank() const = 0;

    /**
     * Set the bank number.
     */
    virtual void setBank(uint8_t bank) = 0;

    /**
     * Get the program number.
     */
    virtual uint8_t getProgram() const = 0;

    /**
     * Set the program number.
     *
     * @post Will mark the bank and program number valid.
     */
    virtual void setProgram(uint8_t program) = 0;

    /**
     * Clear the bank and program number.
     */
    virtual void clearBankAndProgram() = 0;

    /**
     * Get the name.
     */
    virtual std::string getName() const = 0;

    /**
     * Set the name.
     */
    virtual void setName(std::string name) = 0;
};
