//
// Created by Admin on 8/29/2023.
//

#include "overflower.h"
#include <iostream>
#include <cstdint> // For int16_t, int32_t, etc.
#include <limits>  // For std::numeric_limits


class overflower {
public:
static bool additionWillOverflow(int a, int b) {
    int32_t result = static_cast<int32_t>(a) + static_cast<int32_t>(b);
    return result > std::numeric_limits<uint16_t>::max() || result < std::numeric_limits<uint16_t>::min();
}

static bool subtractionWillOverflow(int a, int b) {
    int32_t result = static_cast<int32_t>(a) - static_cast<int32_t>(b);
    return result > std::numeric_limits<uint16_t>::max() || result < std::numeric_limits<uint16_t>::min();
}

static bool multiplicationWillOverflow(int a, int b) {
    int32_t result = static_cast<int32_t>(a) * static_cast<int32_t>(b);
    return result > std::numeric_limits<uint16_t>::max() || result < std::numeric_limits<uint16_t>::min();
}

};