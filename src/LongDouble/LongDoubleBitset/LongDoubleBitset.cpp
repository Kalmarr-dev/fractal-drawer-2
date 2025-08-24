#include "LongDoubleBitset.h"

#include <math.h>
#include "../../helpers/bitset_lt.h"

template<int LENGTH>
LongDoubleBitset<LENGTH>::LongDoubleBitset(double value) {
  this->values = std::bitset<LENGTH>(0);
  double mantissa = std::frexp(value, &this->exponent);
  this->signbit = std::signbit(mantissa);
  mantissa = fabs(mantissa);
  // Remove implicit leading 1: frexp returns mantissa in [0.5, 1.0)
  // So multiply by 2 and subtract 1 to get fractional part
  if (value != 0)
  {
    mantissa *= 2;
    mantissa -= 1.0;
  
    for (int i = 0; i < 52; ++i) {
      mantissa *= 2;
      if (mantissa >= 1.0) {
        values.set(LENGTH - i - 1);  // Fill from MSB to LSB
        mantissa -= 1.0;
      }
    }
  } else {
    this->is_zero = true;
  }
}

template<int LENGTH>
LongDoubleBitset<LENGTH>::LongDoubleBitset(const std::bitset<LENGTH>& values, bool signbit, int exponent, bool is_zero)
  : values(values), signbit(signbit), exponent(exponent), is_zero(is_zero) {}

template<int LENGTH>
double LongDoubleBitset<LENGTH>::get_double(ILongDouble* offset, int scale_exponent) {
  double mantissa = 0.0;

  // Convert bitset to fractional part
  for (int i = 0; i < 52; ++i) {
      if (values.test(LENGTH - i - 1)) {
          mantissa += std::ldexp(1.0, -(i + 1));  // 2^-(i+1)
      }
  }

  // Add the implicit leading 1 (normalized numbers)
  if (!this->is_zero)
  {
    mantissa += 1.0;
  }

  double result = std::ldexp(mantissa, exponent - 1);  // frexp gives mantissa in [0.5, 1), so exponent is biased by +1

  if (signbit)
      result = -result;

  return result;
}

template<int LENGTH>
std::bitset<LENGTH + 2> LongDoubleBitset<LENGTH>::get_full_mantissa() const {
  std::bitset<LENGTH + 2> full(0);
  for (int i = 0; i < LENGTH; ++i) {
    if (values[i]) {
      full.set(i);
    }
  }
  if (!this->is_zero)
  {
    full.set(LENGTH);
  }
  
  return full;
}

template<int LENGTH>
LongDoubleBitset<LENGTH> operator+(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs) {
  std::bitset<LENGTH + 2> new_lhs_values = lhs.get_full_mantissa();
  std::bitset<LENGTH + 2> new_rhs_values = rhs.get_full_mantissa();
  if (lhs.exponent < rhs.exponent) {
    new_lhs_values >>= (rhs.exponent - lhs.exponent);
  } else {
    new_rhs_values >>= (lhs.exponent - rhs.exponent);
  }

  std::bitset<LENGTH + 2> raw_result;
  bool result_sign;

  if (lhs.signbit == rhs.signbit) {
    raw_result = LongDoubleBitset<LENGTH>::add_mantissas(new_lhs_values, new_rhs_values);
    result_sign = lhs.signbit;
  } else {
    // Compare to determine sign
    if (!bitset_lt<LENGTH>(lhs.values, rhs.values)) {
      raw_result = LongDoubleBitset<LENGTH>::subtract_mantissas(new_lhs_values, new_rhs_values);
      result_sign = lhs.signbit;
    } else {
      raw_result = LongDoubleBitset<LENGTH>::subtract_mantissas(new_rhs_values, new_lhs_values);
      result_sign = rhs.signbit;
    }
  }

  int msb = LongDoubleBitset<LENGTH>::find_most_significant_bit(raw_result);
  if (msb == -1) {
    // Zero
    return LongDoubleBitset<LENGTH>(std::bitset<LENGTH>(0), false, 0, true);
  }

  int shift = msb - LENGTH;
  std::bitset<LENGTH + 2> normalized(0);

  if (shift >= 0) {
    for (int i = LENGTH; i >= 0; --i) {
      normalized[i] = raw_result[i + shift];
    }
  } else {
    // TODO test check this logic
    for (int i = LENGTH; i + shift >= 0; --i) {
      normalized[i] = raw_result[i + shift];
    }
  }

  int result_exponent = std::max(lhs.exponent, rhs.exponent) + shift;

  // Remove implicit leading 1(?), store remaining bits
  std::bitset<LENGTH> mantissa;
  for (int i = 0; i < LENGTH; ++i) {
      mantissa[i] = normalized[i];
  }

  return LongDoubleBitset<LENGTH>(mantissa, result_sign, result_exponent, false);
}
