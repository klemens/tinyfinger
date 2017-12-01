const static PROGMEM uint8_t DECODER[] = {
    0b10000001, // 0
    0b11001111, // 1
    0b10010010, // 2
    0b10000110, // 3
    0b11001100, // 4
    0b10100100, // 5
    0b10100000, // 6
    0b10001111, // 7
    0b10000000, // 8
    0b10000100, // 9
    0b11111111, // off
    0b11111110, // -
};

template<uint8_t PIN_SER, uint8_t PIN_SCLK, uint8_t PIN_RCLK>
class SegmentDisplay {
    uint8_t number[2];

public:
    SegmentDisplay() {
        pinMode(PIN_SER, OUTPUT);
        pinMode(PIN_SCLK, OUTPUT);
        pinMode(PIN_RCLK, OUTPUT);
        setOff();
    }

    void set(uint8_t number) {
        uint8_t num = constrain(number, 0, 99);
        this->number[0] = pgm_read_byte_near(DECODER + num % 10);
        if(num / 10 >= 1) {
            this->number[1] = pgm_read_byte_near(DECODER + num / 10);
        } else {
            this->number[1] = pgm_read_byte_near(DECODER + 10);
        }
    }

    void setOff() {
        number[0] = 0xFF;
        number[1] = 0xFF;
    }

    void setDash() {
        number[0] = pgm_read_byte_near(DECODER + 11);
        number[1] = pgm_read_byte_near(DECODER + 11);
    }

    void write() {
        shiftOut(PIN_SER, PIN_SCLK, LSBFIRST, number[0]);
        shiftOut(PIN_SER, PIN_SCLK, LSBFIRST, number[1]);
        digitalWrite(PIN_RCLK, HIGH);
        delay(1);
        digitalWrite(PIN_RCLK, LOW);
    }

    void setDP(bool on) {
        if(on) {
            number[0] &= 0x7F;
            number[1] &= 0x7F;
        } else {
            number[0] |= 0x80;
            number[1] |= 0x80;
        }
    }
};

