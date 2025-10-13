#ifndef PROCESSING_PROCESSINGELEMENT_H_
#define PROCESSING_PROCESSINGELEMENT_H_

#include <map>
#include <string>

#include "JsonConvertible.hpp"
#include "ProcessingTypes.hpp"

namespace processing
{
class Modulatable;
}

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
                         const processing::NoteToLightMap& noteToLightMap) = 0;

    virtual std::map<std::string, processing::Modulatable*> modulationSinks() const;

    enum class Mode
    {
        additive,
        overwriting,
    };
    virtual Mode mode() const;
};

#define MODULATION_SINK_INIT(var) {#var, &var}

#endif /* PROCESSING_PROCESSINGELEMENT_H_ */
