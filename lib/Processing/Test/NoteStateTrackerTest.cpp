#include "../NoteStateTracker.hpp"
#include "Mock/MockMonotonicTime.hpp"
#include "Test/MidiInputObserverTest.hpp"

using ::testing::Each;
using ::testing::NiceMock;
using ::testing::Test;

class NoteStateTrackerTest : public Test, public MidiInputObserverTest
{
  public:
    NiceMock<MockMonotonicTime> time;
    NoteStateTracker tracker{mockMidiInput, time};
    static constexpr MidiInterface::Channel channel{0};
};

MATCHER(IsOff, "note is off")
{
    return !arg.sounding && !arg.pressed;
}

MATCHER_P(IsOnWithVelocity, n, "")
{
    *result_listener << "pressed: " << arg.pressed << ", sounding:" << arg.sounding
                     << ", velocity: " << arg.pressDownVelocity;
    return arg.pressed && arg.sounding && (arg.pressDownVelocity == n);
}

MATCHER_P(IsSustainedWithVelocity, n, "")
{
    *result_listener << "pressed: " << arg.pressed << ", sounding:" << arg.sounding
                     << ", velocity: " << arg.pressDownVelocity;
    return !arg.pressed && arg.sounding && (arg.pressDownVelocity == n);
}

TEST_F(NoteStateTrackerTest, noNotesSounding)
{
    EXPECT_THAT(tracker.noteStates(), Each(IsOff()));
}

TEST_F(NoteStateTrackerTest, noteOn)
{
    // (channel, number, velocity, on/off)
    observer->onNoteChange(channel, 0, 1, true);
    observer->onNoteChange(channel, 5, 6, true);

    tracker.execute();

    EXPECT_THAT(tracker.noteStates()[0], IsOnWithVelocity(1));
    EXPECT_THAT(tracker.noteStates()[5], IsOnWithVelocity(6));
}

TEST_F(NoteStateTrackerTest, noteOff)
{
    // (channel, number, velocity, on/off)
    observer->onNoteChange(channel, 0, 1, true);
    observer->onNoteChange(channel, 5, 6, true);

    observer->onNoteChange(channel, 0, 8, false);

    tracker.execute();

    EXPECT_THAT(tracker.noteStates()[0], IsOff());
    EXPECT_THAT(tracker.noteStates()[5], IsOnWithVelocity(6));
}

TEST_F(NoteStateTrackerTest, ignoreOtherChannel)
{
    observer->onNoteChange(channel + 1, 0, 1, true);

    tracker.execute();
    EXPECT_THAT(tracker.noteStates(), Each(IsOff()));
}

TEST_F(NoteStateTrackerTest, usePedal)
{
    // Press a key
    // (channel, number, velocity, on/off)
    observer->onNoteChange(channel, 0, 1, true);
    // Press pedal
    // (channel, number, value)
    observer->onControlChange(0, MidiInterface::damperPedal, 0xff);
    // Press another key
    observer->onNoteChange(channel, 2, 10, true);

    tracker.execute();
    EXPECT_THAT(tracker.noteStates()[0], IsOnWithVelocity(1));
    EXPECT_THAT(tracker.noteStates()[2], IsOnWithVelocity(10));

    // Release keys
    observer->onNoteChange(channel, 0, 1, false);
    observer->onNoteChange(channel, 2, 10, false);

    tracker.execute();
    EXPECT_THAT(tracker.noteStates()[0], IsSustainedWithVelocity(1));
    EXPECT_THAT(tracker.noteStates()[2], IsSustainedWithVelocity(10));

    // Press another key
    observer->onNoteChange(channel, 1, 5, true);

    tracker.execute();
    EXPECT_THAT(tracker.noteStates()[0], IsSustainedWithVelocity(1));
    EXPECT_THAT(tracker.noteStates()[2], IsSustainedWithVelocity(10));
    EXPECT_THAT(tracker.noteStates()[1], IsOnWithVelocity(5));

    // Release pedal
    observer->onControlChange(0, MidiInterface::damperPedal, 0);

    tracker.execute();
    EXPECT_THAT(tracker.noteStates()[0], IsOff());
    EXPECT_THAT(tracker.noteStates()[1], IsOnWithVelocity(5));
    EXPECT_THAT(tracker.noteStates()[2], IsOff());

    observer->onNoteChange(channel, 1, 0, false);

    tracker.execute();
    EXPECT_THAT(tracker.noteStates()[0], IsOff());
    EXPECT_THAT(tracker.noteStates()[1], IsOff());
    EXPECT_THAT(tracker.noteStates()[2], IsOff());
}
