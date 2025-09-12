#ifndef DRIVERS_MOCK_MOCKMIDIINPUT_H_
#define DRIVERS_MOCK_MOCKMIDIINPUT_H_

#include <gmock/gmock.h>

#include "../Interfaces/IMidiInput.h"

class MockMidiInput : public IMidiInput
{
  public:
    // IMidiInterface implementation
    MOCK_CONST_METHOD0(getPortCount, unsigned int());
    MOCK_METHOD1(openPort, void(int number));

    // IMidiInput implementation
    MOCK_METHOD1(subscribe, void(IObserver& observer));
    MOCK_METHOD1(unsubscribe, void(IObserver& observer));
};

#endif /* DRIVERS_MOCK_MOCKMIDIINPUT_H_ */
