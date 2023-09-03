
#include "overflower.h"
#include <iostream>
#include <cstdint>
#include <limits>


class overflower {
public:
static bool additionWillOverflow(int a, int b) {
    int32_t result = static_cast<int32_t>(a) + static_cast<int32_t>(b);
    return result >  (1 << 16) || result < -(1 << 16);
}

static bool subtractionWillOverflow(int a, int b) {
    int32_t result = static_cast<int32_t>(a) - static_cast<int32_t>(b);
    return result > (1 << 16)  || result < -(1 << 16);
}

static bool multiplicationWillOverflow(int a, int b) {
    int32_t result = static_cast<int32_t>(a) * static_cast<int32_t>(b);
    return result >  (1 << 16) || result < -(1 << 16);
}

};