#ifndef DRIVERS_MOCK_MOCKMIDIINPUT_H_
#define DRIVERS_MOCK_MOCKMIDIINPUT_H_

#include <gmock/gmock.h>

#include "../MidiInput.hpp"

class MockMidiInput : public MidiInput
{
  public:
    // MidiInterface implementation
    MOCK_CONST_METHOD0(getPortCount, unsigned int());
    MOCK_METHOD1(openPort, void(int number));

    // MidiInput implementation
    MOCK_METHOD1(subscribe, void(Observer& observer));
    MOCK_METHOD1(unsubscribe, void(Observer& observer));
};

#endif /* DRIVERS_MOCK_MOCKMIDIINPUT_H_ */
