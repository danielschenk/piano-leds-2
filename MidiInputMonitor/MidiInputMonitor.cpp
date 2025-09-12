#include <cassert>
#include <cstdio>
#include <iostream>

#include "MidiMessageLogger.hpp"
#include "RtMidiMidiInput.hpp"
#include "StdLogger.hpp"

int main()
{
    StdLogger stdLogger;
    RtMidiMidiInput midiInput;
    MidiMessageLogger midiLogger(midiInput);

    int numFoundInputs = midiInput.getPortCount();
    std::cout << "Found " << numFoundInputs << " MIDI inputs.\n";

    if (numFoundInputs > 0)
    {
        midiInput.openPort(0);
        std::cout << "Opened port 0, incoming notes and control changes will be printed to stdout. "
                     "Type <q> <ENTER> to quit.\n";

        while (std::getchar() != 'q')
        {
            // Wait for input
        }

        return 0;
    }
    else
    {
        return 1;
    }
}
