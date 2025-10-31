#pragma once

#include <bitset>

template<int LENGTH>
std::bitset<LENGTH> bitset_subtract(const std::bitset<LENGTH>& a, const std::bitset<LENGTH>& b) {
    std::bitset<LENGTH> result;
    bool borrow = false;
    for (int i = 0; i < LENGTH; ++i) {
        result[i] = a[i] ^ b[i] ^ borrow;
        borrow = (!a[i] && b[i]) || (!a[i] && borrow) || (b[i] && borrow);
    }
    return result;
}