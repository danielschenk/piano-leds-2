#ifndef PROCESSING_PROCESSINGELEMENT_H_
#define PROCESSING_PROCESSINGELEMENT_H_

#include <vector>

#include "JsonConvertible.hpp"
#include "ProcessingTypes.hpp"

class ProcessingBlock : public JsonConvertible
{
  public:
    static constexpr const char* typeNameSingleColorFill = "SingleColorFill";
    static constexpr const char* typeNameNoteVisualizer = "NoteVisualizer";
    static constexpr const char* typeNameProcessingChain = "ProcessingChain";

    virtual ~ProcessingBlock() = default;

    virtual void activate();
    virtual void deactivate();
    virtual void execute(processing::RgbStrip& strip,
                         const processing::TNoteToLightMap& noteToLightMap) = 0;

    enum class Mode
    {
        additive,
        overwriting,
    };
    virtual Mode mode() const;
};

#endif /* PROCESSING_PROCESSINGELEMENT_H_ */
