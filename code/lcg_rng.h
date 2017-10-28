#undef min
#undef max

class lcg_rng {
    uint8_t x = 33, a = 69, c = 85;

    public:
        uint8_t min() const { return 0; }
        uint8_t max() const { return 255; }
        uint8_t operator()() {
            x = a * x + c;
            return x;
        }
};
