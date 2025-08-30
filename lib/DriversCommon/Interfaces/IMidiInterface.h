#ifndef DRIVERS_INTERFACES_IMIDIINTERFACE_H_
#define DRIVERS_INTERFACES_IMIDIINTERFACE_H_

#include <cstdint>

/**
 * Interface for MIDI inputs/outputs.
 */
class IMidiInterface
{
public:
    /** Status type. */
    enum TStatus : uint8_t
    {
        NOTE_OFF = 0x80,
        NOTE_ON = 0x90,
        CONTROL_CHANGE = 0xB0,
        PROGRACHANGE = 0xC0,
        /** Channel pressure is also known as after-touch. */
        CHANNEL_PRESSURE_CHANGE = 0xD0,
        PITCH_BEND_CHANGE = 0xE0,
    };

    /** Controller number type. */
    enum TControllerNumber : uint8_t
    {
        BANK_SELECT_MSB = 0x00,
        BANK_SELECT_LSB = 0x20,
        DAMPER_PEDAL = 0x40,
    };

    static constexpr unsigned int numNotes = 256;
    static constexpr unsigned int maxNoteNumber = 255;
    static constexpr unsigned int numVelocities = 256;
    static constexpr unsigned int maxVelocity = 255;
    static constexpr uint16_t     pitchBendCenter = 0x2000;

    /**
     * Destructor.
     */
    virtual ~IMidiInterface()
    {
    };

    /**
     * Get number of available ports.
     *
     * @return Number of available ports.
     */
    virtual unsigned int getPortCount() const = 0;

    /**
     * Open a port.
     *
     * @param   number  [in]    Port number to open.
     */
    virtual void openPort(int number) = 0;
};



#endif /* DRIVERS_INTERFACES_IMIDIINTERFACE_H_ */
