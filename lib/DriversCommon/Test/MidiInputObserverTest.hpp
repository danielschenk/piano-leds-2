#ifndef MIDIINPUTOBSERVERTEST_H_
#define MIDIINPUTOBSERVERTEST_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../Mock/MockMidiInput.hpp"

using ::testing::_;
using ::testing::NiceMock;

ACTION_P(StoreArg0Address, target)
{
    *target = &arg0;
}

/**
 * Base fixture for tests of classes which implement @ref IMidiInput::IObserver
 */
class MidiInputObserverTest
{
  public:
    MidiInputObserverTest() : mockMidiInput(), observer(nullptr)
    {
        // Capture observer so we can simulate events
        ON_CALL(mockMidiInput, subscribe(_)).WillByDefault(StoreArg0Address(&observer));
    }

    NiceMock<MockMidiInput> mockMidiInput;
    IMidiInput::IObserver* observer;
};

#endif /* MIDIINPUTOBSERVERTEST_H_ */
