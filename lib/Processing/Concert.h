#ifndef PROCESSING_CONCERT_H_
#define PROCESSING_CONCERT_H_

#include <vector>
#include <list>
#include <cstdint>

#include "IJsonConvertible.h"
#include "ProcessingTypes.h"
#include "Scheduler.h"
#include "IMidiInterface.h"
#include "IMidiInput.h"

class IMidiInput;
class IProcessingBlockFactory;
class IPatch;

/**
 * Class which represents a concert.
 *
 * A concert is a collection of patches, together with some settings which are typically constant
 * throughout a gig, like MIDI channels to listen to and the note-to-light mapping.
 */
class Concert
    : public IJsonConvertible
    , public IMidiInput::IObserver
{
public:
    /**
     * Constructor.
     *
     * @param[in]   midiInput               Reference to the MIDI input.
     * @param[in]   processingBlockFactory  Reference to the processing block factory.
     */
    Concert(IMidiInput& midiInput, IProcessingBlockFactory& processingBlockFactory);

    /**
     * Destructor.
     */
    virtual ~Concert();

    // Prevent implicit constructor, copy constructor and assignment operator.
    Concert() = delete;
    Concert(const Concert&) = delete;
    Concert& operator =(const Concert&) = delete;

    // IJsonConvertible implementation
    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& converted);

    typedef int TPatchPosition;
    static constexpr TPatchPosition invalidPatchPosition = -1;

    /**
     * Get the number of patches.
     */
    size_t size() const;

    /**
     * Add a new patch.
     *
     * @return The patch position, or @ref invalidPatchPosition on error.
     */
    TPatchPosition addPatch();

    /**
     * Add an existing patch.
     *
     * @param[in] patch The patch to add.
     *
     * @return The patch position.
     */
    TPatchPosition addPatch(IPatch* patch);

    /**
     * Get the patch at the specified position, for modifications.
     * Concert stays owner of the patch.
     *
     * @param[in] position  The patch position.
     *
     * @return Pointer to the patch, or nullptr on error.
     */
    IPatch* getPatch(TPatchPosition position) const;

    /**
     * Remove the patch at the specified position.
     *
     * @param[in] position  The patch position.
     *
     * @return True on success.
     */
    bool removePatch(TPatchPosition position);

    /**
     * @TODO
     * Move a patch up in the list.
     *
     * @param[in] position  The patch position.
     *
     * @return The new patch position.
     */
    // TPatchPosition movePatchUp(TPatchPosition position);

    /**
     * @TODO
     * Move a patch down in the list.
     *
     * @param[in] position  The patch position.
     *
     * @return The new patch position.
     */
    // TPatchPosition movePatchDown(TPatchPosition position);

    bool isListeningToProgramChange() const;
    void setListeningToProgramChange(bool listeningToProgramChange);
    Processing::TNoteToLightMap getNoteToLightMap() const;
    void setNoteToLightMap(Processing::TNoteToLightMap noteToLightMap);
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
        virtual void onStripUpdate(const Processing::TRgbStrip& strip) = 0;

    protected:
        virtual ~IObserver() = default;
    };

    void subscribe(IObserver& observer);
    void unsubscribe(IObserver& observer);

    // IMidiInput::IObserver implementation
    virtual void onNoteChange(uint8_t channel, uint8_t number, uint8_t velocity, bool on);
    virtual void onProgramChange(uint8_t channel, uint8_t program);
    virtual void onControlChange(uint8_t channel, IMidiInterface::TControllerNumber number, uint8_t value);
    virtual void onChannelPressureChange(uint8_t channel, uint8_t value);
    virtual void onPitchBendChange(uint8_t channel, uint16_t value);

protected:
    // IJsonConvertible implementation
    std::string getObjectType() const;

private:
    static constexpr const char* typeName                             = "Concert";
    static constexpr const char* isListeningToProgramChangeJsonKey    = "isListeningToProgramChange";
    static constexpr const char* noteToLightMapJsonKey                = "noteToLightMap";
    static constexpr const char* programChangeChannelJsonKey          = "programChangeChannel";
    static constexpr const char* currentBankJsonKey                   = "currentBank";
    static constexpr const char* patchesJsonKey                       = "patches";

    typedef std::vector<IPatch*> TPatches;

    TPatchPosition addPatchInternal(IPatch* patch);
    void createMinimumAmountOfLights();

    Processing::TNoteToLightMap noteToLightMap;

    Processing::TRgbStrip strip;
    TPatches patches;
    TPatchPosition activePatchPosition = invalidPatchPosition;
    bool listeningToProgramChange{true};
    uint8_t programChangeChannel = 0;
    uint16_t currentBank = 0;
    IMidiInput& midiInput;
    IProcessingBlockFactory& processingBlockFactory;
    Scheduler scheduler;
    std::list<IObserver*> observers;
    mutable std::mutex mutex;
};

#endif /* PROCESSING_CONCERT_H_ */
