#ifndef SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_
#define SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_

#include <cstdint>

#include "MidiInterface.hpp"

class MidiInput : public MidiInterface
{
  public:
    class Observer
    {
      public:
        virtual void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on);
        virtual void onControlChange(uint8_t channel, ControllerNumber controller, uint8_t value);
        virtual void onProgramChange(uint8_t channel, uint8_t program);
        virtual void onChannelPressureChange(uint8_t channel, uint8_t value);
        virtual void onPitchBendChange(uint8_t channel, uint16_t value);

      protected:
        ~Observer() = default;
    };

    virtual void subscribe(Observer& observer) = 0;
    virtual void unsubscribe(Observer& observer) = 0;
};

#endif /* SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_ */
