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

using MainButton = Button<150, 150>;

Debounce<20> debounce;
MainButton button;

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

enum class State : uint8_t {
    Idle,
    Menu,
    Counting,
    Slowing
};

auto state = State::Idle;
uint16_t timeout;

void loop() {
    rng();

    bool buttonPressed = !digitalRead(PIN1);
    auto event = button.event(debounce.event(buttonPressed));

    switch(state) {
        case State::Idle:
            if(event == ButtonEvent::Press) {
                state = State::Slowing;
            } else if(event == ButtonEvent::LongPress) {
                state = State::Counting;
                timeout = millis();
            } else if(event == ButtonEvent::DoublePress) {
                state = State::Menu;
            }
            break;
        case State::Menu:
            // TODO
            state = State::Idle;
            break;
        case State::Counting:
            if(event == ButtonEvent::LongPress) {
                // stay in this state
                if((uint16_t) millis() - timeout >= 32) {
                    displayNextNumber();
                    timeout = millis();
                }
            } else {
                state = State::Slowing;
            }
            break;
        case State::Slowing:
            slowdown();
            state = State::Idle;
            break;
    }
}

static uint8_t currentNumber;
void displayNextNumber() {
    currentNumber = distribution(rng);
    display.set(currentNumber);
    display.write();
}

void slowdown() {
    for(size_t i = 0; i < NUM_SLOWDOWN; i += 2) {
        for(size_t j = 0; j < pgm_read_word_near(SLOWDOWN + i); ++j) {
            displayNextNumber();
            delay(pgm_read_word_near(SLOWDOWN + i + 1));
        }
    }

    for(uint8_t i = 0; i < 2; ++i) {
        display.setOff();
        display.write();
        delay(250);
        display.set(currentNumber);
        display.write();
        delay(250);
    }
}

