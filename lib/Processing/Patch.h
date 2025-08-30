/**
 * @file
 *
 * MIT License
 *
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @brief Class which represents a patch.
 */

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

    /** Mutex to protect the members. */
    mutable std::mutex mutex;

    /** Whether this patch is bound to a bank and program number. */
    bool bankAndProgramSet = false;

    /** The bank number bound to this patch. */
    uint8_t bank = 0;

    /** The program number bound to this patch. */
    uint8_t program = 0;

    /** The name of the patch. */
    std::string name = "Untitled Patch";

    /** The processing chain. */
    IProcessingChain* processingChain;

    const IProcessingBlockFactory& processingBlockFactory;
};

#endif /* PROCESSING_PATCH_H_ */
