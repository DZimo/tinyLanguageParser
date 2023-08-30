//
// Created by Admin on 8/29/2023.
//

#include "overflower.h"
#include <iostream>
#include <cstdint> // For int16_t, int32_t, etc.
#include <limits>  // For std::numeric_limits


class overflower {
public:
static bool additionWillOverflow(int16_t a, int16_t b) {
    int32_t result = static_cast<int32_t>(a) + static_cast<int32_t>(b);
    return result > std::numeric_limits<int16_t>::max() || result < std::numeric_limits<int16_t>::min();
}

static bool subtractionWillOverflow(int16_t a, int16_t b) {
    int32_t result = static_cast<int32_t>(a) - static_cast<int32_t>(b);
    return result > std::numeric_limits<int16_t>::max() || result < std::numeric_limits<int16_t>::min();
}

static bool multiplicationWillOverflow(int16_t a, int16_t b) {
    int32_t result = static_cast<int32_t>(a) * static_cast<int32_t>(b);
    return result > std::numeric_limits<int16_t>::max() || result < std::numeric_limits<int16_t>::min();
}

};