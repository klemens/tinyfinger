#ifndef _DEBOUNCE_H_
#define _DEBOUNCE_H_


enum class RawButtonEvent : uint8_t {
    NoEvent,
    Pressed,
    Released,
};

template<uint16_t DEBOUNCE_PRESSED, uint16_t DEBOUNCE_UNPRESSED = DEBOUNCE_PRESSED>
class Debounce {
    public:
        enum class State : uint8_t {
            Unpressed,
            UnpressedDebounced,
            Pressed,
            PressedDebounced,
        };


        RawButtonEvent event(bool pressed) {
            switch(state) {
                case State::Unpressed :
                    if(pressed) {
                        state = State::PressedDebounced;
                        counter = millis();
                        return RawButtonEvent::Pressed;
                    }
                    break;
                case State::PressedDebounced :
                    if((uint16_t) millis() - counter > DEBOUNCE_PRESSED) {
                        state = State::Pressed;
                    }
                    break;
                case State::Pressed :
                    if(!pressed) {
                        state = State::UnpressedDebounced;
                        counter = millis();
                        return RawButtonEvent::Released;
                    }
                    break;
                case State::UnpressedDebounced :
                    if((uint16_t) millis() - counter > DEBOUNCE_UNPRESSED) {
                        state = State::Unpressed;
                    }
                    break;
            }

            return RawButtonEvent::NoEvent;
        }

    private:
        State state = State::Unpressed;
        uint16_t counter = 0;
};

#endif
