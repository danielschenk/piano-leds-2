#ifndef ESP32APPLICATION_BOARD_H_
#define ESP32APPLICATION_BOARD_H_

#include "BoardOverride.h"

#ifndef RUN_LED_PIN
#define RUN_LED_PIN 2
#endif

#ifndef DEBUG_RX_PIN
#define DEBUG_RX_PIN 3
#endif

#ifndef DEBUG_TX_PIN
#define DEBUG_TX_PIN 1
#endif

#ifndef MIDI_RX_PIN
#define MIDI_RX_PIN 16
#endif

#ifndef MIDI_TX_PIN
#define MIDI_TX_PIN 17
#endif

#ifndef LED_DATA_PIN
#define LED_DATA_PIN 13
#endif

#ifndef LED_CLOCK_PIN
#define LED_CLOCK_PIN 14
#endif

#endif /* ESP32APPLICATION_BOARD_H_ */
