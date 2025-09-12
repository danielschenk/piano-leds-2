#ifndef DRIVERS_COMMON_BASEMIDIINPUT_H_
#define DRIVERS_COMMON_BASEMIDIINPUT_H_

#include <cstdint>
#include <list>
#include <mutex>
#include <vector>

#include "IMidiInput.hpp"

/**
 * Base class for MIDI inputs, implementing message subscription mechanism.
 */
class BaseMidiInput : public IMidiInput
{
  public:
    /**
     * Destructor.
     */
    virtual ~BaseMidiInput() = default;

    // Prevent implicit copy constructor and assignment operator.
    BaseMidiInput(const BaseMidiInput&) = delete;
    BaseMidiInput& operator=(const BaseMidiInput&) = delete;

    // IMidiInput implementation.
    virtual void subscribe(IObserver& observer);
    virtual void unsubscribe(IObserver& observer);

  protected:
    /**
     * Constructor.
     */
    BaseMidiInput();

    /**
     * Process a single incoming MIDI byte.
     */
    void processMidiByte(uint8_t value);

  private:
    /**
     * Notify observers about a note change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   pitch       The pitch of the note.
     * @param[in]   velocity    The velocity of the note.
     */
    void notifyNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const;

    /**
     * Notify observers about a control change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   controller  The number of the controller.
     * @param[in]   value       The value of the controller.
     */
    void notifyControlChange(uint8_t channel, IMidiInput::ControllerNumber control,
                             uint8_t value) const;

    /**
     * Notify observers about a program change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   program     The program number.
     */
    void notifyProgramChange(uint8_t channel, uint8_t program) const;

    /**
     * Notify observers about a channel pressure (a.k.a. after-touch) change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   value       The new channel pressure value.
     */
    void notifyChannelPressureChange(uint8_t channel, uint8_t value) const;

    /**
     * Notify observers about a pitch bend change.
     *
     * @param[in]   channel     The channel the message was received on.
     * @param[in]   value       The new pitch bend value.
     */
    void notifyPitchBendChange(uint8_t channel, uint16_t value) const;

    /** Collection of observers. */
    std::list<IMidiInput::IObserver*> observers;

    /** Whether incoming bytes are stored to build a message. */
    bool buildingMessage;

    /** The message currently being built. */
    std::vector<uint8_t> currentMessage;

    /** Mutex to protect the observers. */
    mutable std::mutex observersMutex;
};

#endif /* DRIVERS_COMMON_BASEMIDIINPUT_H_ */
