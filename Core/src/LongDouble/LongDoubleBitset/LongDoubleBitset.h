#pragma once

#include <bitset>
#include "../ILongDouble.h"

template<int LENGTH>
class LongDoubleBitset : public ILongDouble
{
private:
public:
  std::bitset<LENGTH> values;
  bool signbit;
  int exponent;
  bool is_zero;

  LongDoubleBitset(double value = 0.0);
  LongDoubleBitset(const std::bitset<LENGTH>& values, bool signbit, int exponent, bool is_zero);
  ~LongDoubleBitset() = default;

  double get_double(ILongDouble* offset, int scale_exponent) override;
  std::bitset<LENGTH + 2> get_full_mantissa() const;
  static int find_most_significant_bit(const std::bitset<LENGTH + 2>& bits) {
    for (int i = LENGTH + 2; i >= 0; --i) {
      if (bits[i]) return i;
    }
    return -1;
  }
  static std::bitset<LENGTH + 2> add_mantissas(const std::bitset<LENGTH + 2>& a, const std::bitset<LENGTH + 2>& b) {
    std::bitset<LENGTH + 2> result;
    bool carry = false;
    for (int i = 0; i < LENGTH + 2; ++i) {
      bool sum = a[i] ^ b[i] ^ carry;
      carry = (a[i] && b[i]) || (a[i] && carry) || (b[i] && carry);
      result[i] = sum;
    }
    return result;
  }
  static std::bitset<LENGTH + 2> subtract_mantissas(const std::bitset<LENGTH + 2>& a, const std::bitset<LENGTH + 2>& b) {
    std::bitset<LENGTH + 2> result;
    bool borrow = false;
    for (int i = 0; i < LENGTH + 2; ++i) {
        result[i] = a[i] ^ b[i] ^ borrow;
        borrow = (!a[i] && b[i]) || (!a[i] && borrow) || (b[i] && borrow);
    }
    return result;
  }
};

template<int LENGTH>
bool operator<(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs);
template<int LENGTH>
bool operator==(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs);

template<int LENGTH>
LongDoubleBitset<LENGTH> operator+(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs);
template<int LENGTH>
LongDoubleBitset<LENGTH> operator-(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs);

template<int LENGTH>
LongDoubleBitset<LENGTH> operator*(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs);
template<int LENGTH>
LongDoubleBitset<LENGTH> operator/(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs);

#define _LongDoubleBitset LongDoubleBitset<64>