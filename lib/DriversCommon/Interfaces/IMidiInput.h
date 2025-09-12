#ifndef SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_
#define SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_

#include <cstdint>

#include "IMidiInterface.h"

class IMidiInput : public IMidiInterface
{
  public:
    /**
     * Interface to implement by MIDI input observers.
     */
    class IObserver
    {
      public:
        /**
         * Called when a note on/off message is received.
         *
         * @param channel       Channel number
         * @param pitch         Pitch (note number)
         * @param velocity      Velocity
         * @param on            True = note on, false = note off
         */
        virtual void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) = 0;

        /**
         * Called when a control change message is received.
         *
         * @param channel       Channel number
         * @param controller    Controller number
         * @param value         New value of the changed control
         */
        virtual void onControlChange(uint8_t channel, TControllerNumber controller,
                                     uint8_t value) = 0;

        /**
         * Called when a program change message is received.
         *
         * @param channel       Channel number
         * @param program       Number of the new active program
         */
        virtual void onProgramChange(uint8_t channel, uint8_t program) = 0;

        /**
         * Called when a channel pressure (a.k.a. after-touch) message is received.
         *
         * @param channel       Channel number
         * @param value         New channel pressure value
         */
        virtual void onChannelPressureChange(uint8_t channel, uint8_t value) = 0;

        /**
         * Called when a pitch bend message is received.
         *
         * @param channel       Channel number
         * @param value         New pitch bend value
         */
        virtual void onPitchBendChange(uint8_t channel, uint16_t value) = 0;

      protected:
        /**
         * Destructor.
         */
        ~IObserver() = default;
    };

    /**
     * Subscribe to MIDI events.
     *
     * @param   observer    The observer to subscribe
     */
    virtual void subscribe(IObserver& observer) = 0;

    /**
     * Unsubscribe from MIDI events.
     *
     * @param   observer    The observer to unsubscribe
     */
    virtual void unsubscribe(IObserver& observer) = 0;

  protected:
    /**
     * Destructor.
     */
    virtual ~IMidiInput() = default;
};

#endif /* SOURCE_DRIVERS_INCLUDE_IMIDIINPUT_H_ */
