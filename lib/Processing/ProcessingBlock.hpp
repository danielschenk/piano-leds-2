#ifndef PROCESSING_PROCESSINGELEMENT_H_
#define PROCESSING_PROCESSINGELEMENT_H_

#include <array>
#include <functional>
#include <vector>

#include "JsonConvertible.hpp"
#include "MidiInterface.hpp"
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

    struct Input
    {
        processing::Timestamp nowMs;
        processing::NoteToLightMap& noteToLightMap;
        std::vector<std::reference_wrapper<const processing::NoteStates>> noteStates;
    };
    virtual bool valid(const Input& input);
    virtual void execute(processing::RgbStrip& strip, const Input& input) = 0;

    enum class Mode
    {
        additive,
        overwriting,
    };
    virtual Mode mode() const;
};

#endif /* PROCESSING_PROCESSINGELEMENT_H_ */
