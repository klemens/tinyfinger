// Conflicts with std c++ headers
#undef min
#undef max

#include "button.h"
#include "debounce.h"
#include "lcg_rng.h"
#include "segment.h"
#include "uniform_int_dist.h"


#define BUTTON_PIN 1
#define DISPLAY_ENABLE 3

lcg_rng rng;
std::uniform_int_distribution<uint8_t> distribution(1,16);

SegmentDisplay<4, 2, 0> display;

#define NUM_SLOWDOWN 12
const uint16_t PROGMEM SLOWDOWN[NUM_SLOWDOWN] = {
    35, 32,
    20, 64,
     8, 125,
     4, 250,
     2, 500,
     2, 1000
};

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(DISPLAY_ENABLE, OUTPUT);
    digitalWrite(DISPLAY_ENABLE, 0);

    display.setDash();
    display.write();
}

void loop() {
    if(digitalRead(PIN1)) {
        rng();
        return;
    }

    uint8_t num;

    for(size_t i = 0; i < NUM_SLOWDOWN; i += 2) {
        for(size_t j = 0; j < pgm_read_word_near(SLOWDOWN + i); ++j) {
            num = distribution(rng);
            display.set(num);
            display.write();
            delay(pgm_read_word_near(SLOWDOWN + i + 1));
        }
    }

    for(uint8_t i = 0; i < 2; ++i) {
        display.setOff();
        display.write();
        delay(250);
        display.set(num);
        display.write();
        delay(250);
    }
}

