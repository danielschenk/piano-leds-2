#ifndef DRIVERS_MIDIINTERFACE_H_
#define DRIVERS_MIDIINTERFACE_H_

#include <cstdint>

class MidiInterface
{
  public:
    enum Status : uint8_t
    {
        noteOff = 0x80,
        noteOn = 0x90,
        controlChange = 0xB0,
        programChange = 0xC0,
        // Channel pressure is also known as after-touch
        channelPressureChange = 0xD0,
        pitchBendChange = 0xE0,
    };

    enum ControllerNumber : uint8_t
    {
        bankSelectMsb = 0x00,
        bankSelectLsb = 0x20,
        damperPedal = 0x40,
    };

    static constexpr unsigned int numNotes = 256;
    static constexpr unsigned int maxNoteNumber = 255;
    static constexpr unsigned int numVelocities = 256;
    static constexpr unsigned int maxVelocity = 255;
    static constexpr uint16_t pitchBendCenter = 0x2000;

    using Channel = uint8_t;
    using NoteNumber = uint8_t;
    using Velocity = uint8_t;

    virtual ~MidiInterface() = default;
    virtual unsigned int getPortCount() const = 0;
    virtual void openPort(int number) = 0;
};

#endif /* DRIVERS_INTERFACES_IMIDIINTERFACE_H_ */
