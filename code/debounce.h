enum class State : uint8_t {
    Unpressed,
    UnpressedDebounced,
    Pressed,
    PressedDebounced,
};

template<uint8_t DEBOUNCE_PRESSED, uint8_t DEBOUNCE_UNPRESSED = DEBOUNCE_PRESSED>
class Debounce {
    
};

