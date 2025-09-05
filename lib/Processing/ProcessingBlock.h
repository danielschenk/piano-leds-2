#ifndef PROCESSING_PROCESSINGELEMENT_H_
#define PROCESSING_PROCESSINGELEMENT_H_

#include <vector>

#include "ProcessingTypes.h"
#include "JsonConvertible.h"

class ProcessingBlock
    : public JsonConvertible
{
public:
    static constexpr const char* typeNameSingleColorFill  = "SingleColorFill";
    static constexpr const char* typeNameNoteRgbSource        = "NoteRgbSource";
    static constexpr const char* typeNameProcessingChain      = "ProcessingChain";

    virtual ~ProcessingBlock() = default;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap) = 0;

    enum class Mode
    {
        additive,
        overwriting,
    };
    virtual Mode mode() const;
};

#endif /* PROCESSING_PROCESSINGELEMENT_H_ */
