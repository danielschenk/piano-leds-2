#ifndef DRIVERS_COMMON_BASEMIDIINPUT_H_
#define DRIVERS_COMMON_BASEMIDIINPUT_H_

#include <cstdint>
#include <list>
#include <mutex>
#include <vector>

#include "MidiInput.hpp"

class BaseMidiInput : public MidiInput
{
  public:
    virtual ~BaseMidiInput() = default;

    // Prevent implicit copy constructor and assignment operator.
    BaseMidiInput(const BaseMidiInput&) = delete;
    BaseMidiInput& operator=(const BaseMidiInput&) = delete;

    // MidiInput implementation.
    virtual void subscribe(IObserver& observer);
    virtual void unsubscribe(IObserver& observer);

  protected:
    BaseMidiInput();
    void processMidiByte(uint8_t value);

  private:
    void notifyNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const;
    void notifyControlChange(uint8_t channel, MidiInput::ControllerNumber control,
                             uint8_t value) const;
    void notifyProgramChange(uint8_t channel, uint8_t program) const;
    void notifyChannelPressureChange(uint8_t channel, uint8_t value) const;
    void notifyPitchBendChange(uint8_t channel, uint16_t value) const;

    std::list<MidiInput::IObserver*> observers;
    bool buildingMessage{false};
    std::vector<uint8_t> currentMessage;
    mutable std::mutex observersMutex;
};

#endif /* DRIVERS_COMMON_BASEMIDIINPUT_H_ */
