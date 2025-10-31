#pragma once

#include <bitset>

template<int LENGTH>
bool bitset_lt(const std::bitset<LENGTH>& a, const std::bitset<LENGTH>& b) {
  for (int i = LENGTH - 1; i >= 0; --i) {
    if (a[i] != b[i]) {
      return b[i];
    }
  }
  return false;
}

// template bool bitset_lt<1024>(const std::bitset<1024>& a, const std::bitset<1024>& b);