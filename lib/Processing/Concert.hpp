#ifndef PROCESSING_CONCERT_H_
#define PROCESSING_CONCERT_H_

#include <cstdint>
#include <list>
#include <vector>

#include "JsonConvertible.hpp"
#include "MidiInput.hpp"
#include "MidiInterface.hpp"
#include "NoteStateTracker.hpp"
#include "Platform.hpp"
#include "ProcessingTypes.hpp"
#include "Scheduler.hpp"

class MidiInput;
class IProcessingBlockFactory;
class IPatch;

/**
 * Class which represents a concert.
 *
 * A concert is a collection of patches, together with some settings which are typically constant
 * throughout a gig, like MIDI channels to listen to and the note-to-light mapping.
 */
class Concert : public JsonConvertible, public MidiInput::Observer
{
  public:
    Concert(MidiInput& midiInput, IProcessingBlockFactory& processingBlockFactory,
            const MonotonicTime& time);
    ~Concert() override;

    Concert() = delete;
    Concert(const Concert&) = delete;
    Concert& operator=(const Concert&) = delete;

    // JsonConvertible implementation
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

    typedef int PatchPosition;
    static constexpr PatchPosition invalidPatchPosition = -1;

    size_t size() const;
    PatchPosition addPatch();
    PatchPosition addPatch(IPatch* patch);
    IPatch* getPatch(PatchPosition position) const;
    bool removePatch(PatchPosition position);

    void reactivateLastActivePatch();

    // TODO
    // PatchPosition movePatchUp(PatchPosition position);

    // TODO
    // PatchPosition movePatchDown(PatchPosition position);

    processing::NoteToLightMap getNoteToLightMap() const;
    void setNoteToLightMap(processing::NoteToLightMap noteToLightMap);
    size_t getStripSize() const;
    void setStripSize(size_t size);
    uint8_t getProgramChangeChannel() const;
    void setProgramChangeChannel(uint8_t programChangeChannel);
    uint16_t getCurrentBank() const;
    void setCurrentBank(uint16_t bank);

    void execute();

    class IObserver
    {
      public:
        virtual void onStripUpdate(const processing::RgbStrip& strip) = 0;

      protected:
        virtual ~IObserver() = default;
    };

    void subscribe(IObserver& observer);
    void unsubscribe(IObserver& observer);

    // MidiInput::Observer implementation
    void onProgramChange(uint8_t channel, uint8_t program) override;
    void onControlChange(uint8_t channel, MidiInterface::ControllerNumber number,
                         uint8_t value) override;

  protected:
    // JsonConvertible implementation
    std::string getObjectType() const override;

  private:
    static constexpr const char* typeName = "Concert";
    static constexpr const char* noteToLightMapJsonKey = "noteToLightMap";
    static constexpr const char* programChangeChannelJsonKey = "programChangeChannel";
    static constexpr const char* currentBankJsonKey = "currentBank";
    static constexpr const char* patchesJsonKey = "patches";

    typedef std::vector<IPatch*> Patches;

    PatchPosition addPatchInternal(IPatch* patch);
    void createMinimumAmountOfLights();

    processing::NoteToLightMap noteToLightMap;

    processing::RgbStrip strip;
    Patches patches;
    PatchPosition activePatchPosition = invalidPatchPosition;
    uint8_t programChangeChannel = 0;
    uint16_t currentBank = 0;
    MidiInput& midiInput;
    IProcessingBlockFactory& processingBlockFactory;
    Scheduler scheduler;
    std::list<IObserver*> observers;
    NoteStateTracker noteStateTracker;
    mutable std::mutex mutex;

    // use an arbitrary mask instead of XOR'ing all bits, to reduce the likelihood of an accidental
    // false positive
    static constexpr uint8_t lastProgramChangeXorMask = 0x5C;
    static uint8_t lastProgramChange NOINIT;
    static uint8_t lastProgramChangeXorCheck NOINIT;
};

#endif /* PROCESSING_CONCERT_H_ */
