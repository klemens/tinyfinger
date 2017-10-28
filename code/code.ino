// Conflicts with std c++ headers
#undef min
#undef max

#include "debounce.h"
#include "lcg_rng.h"
#include "segment.h"
#include "uniform_int_dist.h"


#define PIN1 1

lcg_rng rng;
std::uniform_int_distribution<uint8_t> distribution(1,16);

SegmentDisplay<4, 2, 0> display;

void setup() {
    pinMode(PIN1, INPUT_PULLUP);
    pinMode(3, OUTPUT);
    digitalWrite(3, 0);
    digitalWrite(0, 0);
}

void loop() {
    if(!digitalRead(PIN1)) {
        display.set(distribution(rng));
        display.write();
    }
    delay(16);
}

