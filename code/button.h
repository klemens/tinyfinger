#include "debounce.h"


enum class ButtonEvent : uint8_t {
    NoEvent,
    Press,
    LongPress,
    DoublePress,
};

// template wird komplett wegoptimiert (wie #define)
template<uint16_t LONGPRESS_TIMEOUT, uint16_t DOUBLEPRESS_TIMEOUT>
class Button {
    public:
        enum class State : uint8_t {
            Idle,
            Pressed,
            LongPressed,
            Released,
            DoublePressed,
        };


        ButtonEvent event(RawButtonEvent result) {
            switch(state) {
                case State::Idle :
                    if(result == RawButtonEvent::Pressed) {
                        state = State::Pressed;
                        counter = millis();
                    }
                    break;
                case State::Pressed :
                    if(result == RawButtonEvent::Released) {
                        state = State::Released;
                        counter = millis();
                    } else if(millis() - counter >= LONGPRESS_TIMEOUT) {
                        state = State::LongPressed;
                        return ButtonEvent::LongPress;
                    }
                    break;
                case State::Released :
                    if(result == RawButtonEvent::Pressed) {
                        state = State::DoublePressed;
                    } else if(millis() - counter >= DOUBLEPRESS_TIMEOUT) {
                        state = State::Idle;
                        counter = millis();
                        return ButtonEvent::Press;
                    }
                    break;
                case State::LongPressed :
                    if(result == RawButtonEvent::Released) {
                        state = State::Idle;
                        counter = millis();
                    } else {
                        return ButtonEvent::LongPress;
                    }
                    break;
                case State::DoublePressed :
                    if(result == RawButtonEvent::Released) {
                        state = State::Idle;
                        counter = millis();
                        return ButtonEvent::DoublePress;
                    }
                    break;
            }

            return ButtonEvent::NoEvent;
        }

    private:
        State state = State::Idle;
        uint16_t counter = 0;
};
