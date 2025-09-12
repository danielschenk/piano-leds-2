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
        virtual void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) = 0;
        virtual void onControlChange(uint8_t channel, ControllerNumber controller,
                                     uint8_t value) = 0;
        virtual void onProgramChange(uint8_t channel, uint8_t program) = 0;
        virtual void onChannelPressureChange(uint8_t channel, uint8_t value) = 0;
        virtual void onPitchBendChange(uint8_t channel, uint16_t value) = 0;

      protected:
        ~Observer() = default;
    };

    virtual void subscribe(Observer& observer) = 0;
    virtual void unsubscribe(Observer& observer) = 0;

  protected:
    virtual ~MidiInput() = default;
};

#endif /* SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_ */
