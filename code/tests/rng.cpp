#include <iostream>
#include <cstdint>
#include <map>

#include "../lcg_rng.h"
#include "../uniform_int_dist.h"


int main() {
    lcg_rng rng;
    std::uniform_int_distribution<uint8_t> distribution(1,8);

    std::map<uint8_t, size_t> map;
    for(int i=0;i<=25500000;++i) {
        uint8_t x = distribution(rng);
        map[x] += 1;
        //std::cout << (int) x << std::endl;
    }

    for(auto x : map) {
        std::cout << (int) x.first << ", " << x.second << std::endl;
    }

    return 0;
}
