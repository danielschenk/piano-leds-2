#ifndef PROCESSING_CONCERT_H_
#define PROCESSING_CONCERT_H_

#include <cstdint>
#include <list>
#include <vector>

#include "JsonConvertible.hpp"
#include "MidiInput.hpp"
#include "MidiInterface.hpp"
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
class Concert : public JsonConvertible, public MidiInput::IObserver
{
  public:
    /**
     * Constructor.
     *
     * @param[in]   midiInput               Reference to the MIDI input.
     * @param[in]   processingBlockFactory  Reference to the processing block factory.
     */
    Concert(MidiInput& midiInput, IProcessingBlockFactory& processingBlockFactory);

    /**
     * Destructor.
     */
    virtual ~Concert();

    // Prevent implicit constructor, copy constructor and assignment operator.
    Concert() = delete;
    Concert(const Concert&) = delete;
    Concert& operator=(const Concert&) = delete;

    // JsonConvertible implementation
    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& converted);

    typedef int PatchPosition;
    static constexpr PatchPosition invalidPatchPosition = -1;

    /**
     * Get the number of patches.
     */
    size_t size() const;

    /**
     * Add a new patch.
     *
     * @return The patch position, or @ref invalidPatchPosition on error.
     */
    PatchPosition addPatch();

    /**
     * Add an existing patch.
     *
     * @param[in] patch The patch to add.
     *
     * @return The patch position.
     */
    PatchPosition addPatch(IPatch* patch);

    /**
     * Get the patch at the specified position, for modifications.
     * Concert stays owner of the patch.
     *
     * @param[in] position  The patch position.
     *
     * @return Pointer to the patch, or nullptr on error.
     */
    IPatch* getPatch(PatchPosition position) const;

    /**
     * Remove the patch at the specified position.
     *
     * @param[in] position  The patch position.
     *
     * @return True on success.
     */
    bool removePatch(PatchPosition position);

    /**
     * @TODO
     * Move a patch up in the list.
     *
     * @param[in] position  The patch position.
     *
     * @return The new patch position.
     */
    // PatchPosition movePatchUp(PatchPosition position);

    /**
     * @TODO
     * Move a patch down in the list.
     *
     * @param[in] position  The patch position.
     *
     * @return The new patch position.
     */
    // PatchPosition movePatchDown(PatchPosition position);

    bool isListeningToProgramChange() const;
    void setListeningToProgramChange(bool listeningToProgramChange);
    processing::NoteToLightMap getNoteToLightMap() const;
    void setNoteToLightMap(processing::NoteToLightMap noteToLightMap);
    size_t getStripSize() const;
    uint8_t getProgramChangeChannel() const;
    void setProgramChangeChannel(uint8_t programChangeChannel);
    uint16_t getCurrentBank() const;
    void setCurrentBank(uint16_t bank);

    void execute();

    /**
     * Interface to implement by Concert observers.
     */
    class IObserver
    {
      public:
        virtual void onStripUpdate(const processing::RgbStrip& strip) = 0;

      protected:
        virtual ~IObserver() = default;
    };

    void subscribe(IObserver& observer);
    void unsubscribe(IObserver& observer);

    // MidiInput::IObserver implementation
    virtual void onNoteChange(uint8_t channel, uint8_t number, uint8_t velocity, bool on);
    virtual void onProgramChange(uint8_t channel, uint8_t program);
    virtual void onControlChange(uint8_t channel, MidiInterface::ControllerNumber number,
                                 uint8_t value);
    virtual void onChannelPressureChange(uint8_t channel, uint8_t value);
    virtual void onPitchBendChange(uint8_t channel, uint16_t value);

  protected:
    // JsonConvertible implementation
    std::string getObjectType() const;

  private:
    static constexpr const char* typeName = "Concert";
    static constexpr const char* isListeningToProgramChangeJsonKey = "isListeningToProgramChange";
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
    bool listeningToProgramChange{true};
    uint8_t programChangeChannel = 0;
    uint16_t currentBank = 0;
    MidiInput& midiInput;
    IProcessingBlockFactory& processingBlockFactory;
    Scheduler scheduler;
    std::list<IObserver*> observers;
    mutable std::mutex mutex;
};

#endif /* PROCESSING_CONCERT_H_ */
