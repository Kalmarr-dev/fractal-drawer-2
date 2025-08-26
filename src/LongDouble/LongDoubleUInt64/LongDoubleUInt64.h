#pragma once

#include <array>
#include "../ILongDouble.h"

template<int LENGTH>
class LongDoubleUInt64 : public ILongDouble
{
private:
public:
  std::array<uint64_t, LENGTH> values;
  bool signbit;
  int exponent;
  bool is_zero;

  LongDoubleUInt64(double value = 0.0);
  LongDoubleUInt64(const std::array<uint64_t, LENGTH>& values, bool signbit, int exponent, bool is_zero);
  ~LongDoubleUInt64() = default;

  double get_double(ILongDouble* offset, int scale_exponent) override;
};

template<int LENGTH>
bool operator<(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs);
template<int LENGTH>
bool operator==(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs);

template<int LENGTH>
LongDoubleUInt64<LENGTH> operator+(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs);
template<int LENGTH>
LongDoubleUInt64<LENGTH> operator-(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs);

template<int LENGTH>
LongDoubleUInt64<LENGTH> operator*(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs);
template<int LENGTH>
LongDoubleUInt64<LENGTH> operator/(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs);