#ifndef PROCESSING_IPROCESSINGBLOCK_H_
#define PROCESSING_IPROCESSINGBLOCK_H_

#include <vector>

#include "ProcessingTypes.h"
#include "IJsonConvertible.h"

/**
 * Interface for processing blocks.
 */
class IProcessingBlock
    : public IJsonConvertible
{
public:
    static constexpr const char* typeNameEqualRangeRgbSource  = "EqualRangeRgbSource";
    static constexpr const char* typeNameNoteRgbSource        = "NoteRgbSource";
    static constexpr const char* typeNameProcessingChain      = "ProcessingChain";

    /**
     * Destructor.
     */
    virtual ~IProcessingBlock() = default;

    /**
     * Activate this block.
     *
     * @post The block responds to events.
     */
    virtual void activate() = 0;

    /**
     * Deactivate this block.
     *
     * @post The block ignores events.
     * @post The block is in a clean state (as if no events have ever been received).
     */
    virtual void deactivate() = 0;

    /**
     * Execute this block on the given strip.
     *
     * @param   [in/out]    strip           The strip to operate on.
     * @param   [in/out]    noteToLightMap  To map from note number to light number.
     */
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap) = 0;
};

#endif /* PROCESSING_IPROCESSINGBLOCK_H_ */
