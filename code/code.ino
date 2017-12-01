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

bool activeGroups[16] = { false };
uint8_t activeCount = 0;

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

    resetGroups();
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
            displayMenu();
            display.setDash();
            display.write();
            state = State::Idle;
            break;
        case State::Counting:
            if(event == ButtonEvent::LongPress) {
                // stay in this state
                if((uint16_t) millis() - timeout >= 25) {
                    display.set(generateNextNumber());
                    display.write();
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

uint8_t generateNextNumber() {
    uint8_t randomNumber = distribution(rng);

    uint8_t i;
    for(i = 0; i < 16 && randomNumber != 0; i += 1) {
        if(activeGroups[i]) {
            randomNumber -= 1;
        }
    }

    // already offset by 1 because of the loop postincrement
    return i;
}

void slowdown() {
    uint8_t currentNumber;

    for(size_t i = 0; i < NUM_SLOWDOWN; i += 2) {
        for(size_t j = 0; j < pgm_read_word_near(SLOWDOWN + i); ++j) {
            currentNumber = generateNextNumber();
            display.set(currentNumber);
            display.write();
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

void resetGroups() {
    for(uint8_t i = 0; i < 16; i += 1) {
        activeGroups[i] = true;
        activeCount += 1;
    }
}

void displayMenu() {
    uint8_t currentGroup = 0;
    uint16_t timeout = millis();

    constexpr uint16_t longPressDelay = 300;
    Button<longPressDelay, 150> menuButton;

    for(;;) {
        bool buttonPressed = !digitalRead(PIN1);
        auto event = menuButton.event(debounce.event(buttonPressed));

        display.set(currentGroup + 1);
        display.setDP(activeGroups[currentGroup]);
        display.write();

        switch(event) {
            case ButtonEvent::LongPress:
                // rotate through groups
                if((uint16_t) millis() - timeout >= longPressDelay) {
                    currentGroup = (currentGroup + 1) % 16;
                    timeout = millis();
                }
                break;
            case ButtonEvent::Press:
                // (de)activate group
                activeGroups[currentGroup] = !activeGroups[currentGroup];
                break;
            case ButtonEvent::DoublePress:
                // exit menu
                activeCount = 0;
                for(uint8_t i = 0; i < 16; i += 1) {
                    activeCount += (uint8_t) activeGroups[i];
                }
                if(activeCount == 0) {
                    resetGroups();
                }

                distribution = std::uniform_int_distribution<uint8_t>(1,activeCount);
                return;
        }
    }
}
