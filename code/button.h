#include "debounce.h"

template<uint16_t SLEEP_TIMEOUT, uint16_t LONGPRESS_TIMEOUT, uint16_t DOUBLEPRESS_TIMEOUT>
class Button {

    using DResult = Debounce<0>::Result;

    public:
        enum class State : uint8_t {
            Sleep,
            Idle,
            Pressed,
            LongPressed,
            Released,
            DoublePressed,
        };

        enum class Result : uint8_t {
            NoEvent,
            Press,
            LongPress,
            DoublePress,
        };

        Result event(DResult result) {
            switch(state) {
                case State::Sleep :
                    state = State::Idle;
                    counter = millis();
                    break;
                case State::Idle :
                    if(result == DResult::Pressed) {
                        state = State::Pressed;
                        counter = millis();
                    } else if(millis() - counter >= SLEEP_TIMEOUT) {
                        state = State::Sleep;
                        // Activate Hardware sleep
                    }
                    break;
                case State::Pressed :
                    if(result == DResult::Released) {
                        state = State::Released;
                        counter = millis();
                    } else if(millis() - counter >= LONGPRESS_TIMEOUT) {
                        state = State::LongPressed;
                        return Result::LongPress;
                    }
                    break;
                case State::Released :
                    if(result == DResult::Pressed) {
                        state = State::DoublePressed;
                    } else if(millis() - counter >= DOUBLEPRESS_TIMEOUT) {
                        state = State::Idle;
                        counter = millis();
                        return Result::Press;
                    }
                    break;
                case State::LongPressed :
                    if(result == DResult::Released) {
                        state = State::Idle;
                        counter = millis();
                    } else {
                        return Result::LongPress;
                    }
                    break;
                case State::DoublePressed :
                    if(result == DResult::Released) {
                        state = State::Idle;
                        counter = millis();
                        return Result::DoublePress;
                    }
                    break;
            }

            return Result::NoEvent;
        }

    private:
        State state = State::Idle;
        uint16_t counter = 0;
};
