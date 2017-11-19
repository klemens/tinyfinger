#ifndef _DEBOUNCE_H_
#define _DEBOUNCE_H_


enum RawButtonEvent : uint8_t {
    Released = 0,
    Pressed = 1,
    NoEvent = 2,
};

template<uint8_t DEBOUNCE_TIMEOUT>
class Debounce {
    public:
        enum State : uint8_t {
            Unpressed = 0,
            PressedDebounced = 1,
            Pressed = 2,
            UnpressedDebounced = 3,
        };


        RawButtonEvent event(bool currentButtonState) {
            if(state & 0b01) { // PressedDebounced, UnpressedDebounced
                // Leave debounce states after debounce timeout
                if(millis() - counter > DEBOUNCE_TIMEOUT) {
                    state = (State) ((uint8_t) (state + 1) % 4);
                }
            } else { // Unpressed, Pressed
                // Go to debounce when state changes:
                // state (Pressed/Unpressed) != currentButtonState (true/false)
                // -> different states always add up to 2 (0b10), same ones to 0 or 4 (0b100)
                if((((uint8_t)state & 0b10) + ((uint8_t)currentButtonState * 2)) & 2) {
                    counter = millis();
                    state = (State) (state + 1); // no mod4 necessary
                    return (RawButtonEvent) currentButtonState;
                }
            }

            return RawButtonEvent::NoEvent;
        }

    private:
        State state = State::Unpressed;
        uint8_t counter = 0;
};

#endif
