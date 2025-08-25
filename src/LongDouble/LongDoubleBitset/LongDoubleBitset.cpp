#include "LongDoubleBitset.h"

#include <math.h>
#include <stdexcept>
#include "../../helpers/bitset_lt.h"
#include "../../helpers/bitset_subtract.h"

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
    this->is_zero = false;
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
  // TODO consider offset !!!
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
bool operator<(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs) {
  if (lhs.signbit != rhs.signbit) {
    return lhs.signbit; // true if lhs is negative and rhs is positive
  }

  if (lhs.is_zero && rhs.is_zero) return false;
  if (lhs.is_zero) return !rhs.signbit;
  if (rhs.is_zero) return lhs.signbit;

  // Both positive or both negative
  bool abs_less_than;

  if (lhs.exponent != rhs.exponent) {
    abs_less_than = lhs.exponent < rhs.exponent;
  } else {
    std::bitset<LENGTH + 2> lhs_mant = lhs.get_full_mantissa();
    std::bitset<LENGTH + 2> rhs_mant = rhs.get_full_mantissa();

    for (int i = LENGTH + 1; i >= 0; --i) {
      if (lhs_mant[i] != rhs_mant[i]) {
        abs_less_than = !lhs_mant[i] && rhs_mant[i];
        break;
      }
    }

    // If identical, not less than
    if (lhs.get_full_mantissa() == rhs.get_full_mantissa()) {
        return false;
    }
  }

  return lhs.signbit ? !abs_less_than : abs_less_than;
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

  std::bitset<LENGTH + 2> raw_result(0);
  bool result_sign;

  if (lhs.signbit == rhs.signbit) {
    raw_result = LongDoubleBitset<LENGTH>::add_mantissas(new_lhs_values, new_rhs_values);
    result_sign = lhs.signbit;
  } else {
    // Compare to determine sign
    if (!bitset_lt<LENGTH + 2>(new_lhs_values, new_rhs_values)) {
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

template<int LENGTH>
LongDoubleBitset<LENGTH> operator-(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs) {
  auto new_rhs = LongDoubleBitset<LENGTH>(rhs.values, !rhs.signbit, rhs.exponent, rhs.is_zero);
  return lhs + new_rhs;
}

template<int LENGTH>
LongDoubleBitset<LENGTH> operator*(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs) {
  if (lhs.is_zero || rhs.is_zero) {
    return LongDoubleBitset<LENGTH>(
      std::bitset<LENGTH>(0), 
      lhs.signbit ^ rhs.signbit, 
      0,
      true
    );
  }

  bool result_sign = lhs.signbit ^ rhs.signbit;

  int result_exponent = lhs.exponent + rhs.exponent;

  std::bitset<LENGTH + 2> lhs_mant = lhs.get_full_mantissa();
  std::bitset<LENGTH + 2> rhs_mant = rhs.get_full_mantissa();

  constexpr int FULL = LENGTH + 2;
  constexpr int RESULT_BITS = 2 * FULL;
  std::bitset<RESULT_BITS> product(0);

  
  for (int i = 0; i < FULL; ++i) {
    if (rhs_mant[i]) {
      std::bitset<RESULT_BITS> shifted_lhs(0);
      for (int j = 0; j < FULL; ++j) {
        if (lhs_mant[j]) {
          shifted_lhs[i + j] = 1;
        }
      }

      bool carry = false;
      for (int k = 0; k < RESULT_BITS; ++k) {
        bool a = product[k];
        bool b = shifted_lhs[k];
        bool sum = a ^ b ^ carry;
        carry = (a && b) || (a && carry) || (b && carry);
        product[k] = sum;
      }
    }
  }

  // SECTION Normalize the result
  int msb = -1;
  for (int i = RESULT_BITS - 1; i >= 0; --i) {
    if (product[i]) {
      msb = i;
      break;
    }
  }

  if (msb == -1) {
    // Product is zero (shouldn't happen if both are non-zero, but safe fallback)
    return LongDoubleBitset<LENGTH>(std::bitset<LENGTH>(0), result_sign, 0, true);
  }

  int shift = msb - (LENGTH);
  std::bitset<RESULT_BITS> normalized;
  if (shift >= 0) {
    normalized = product >> shift;
  } else {
    normalized = product << (-shift);
  }

  // Adjust exponent due to normalization
  result_exponent += shift - LENGTH - 1;

  // SECTION Extract mantissa bits (drop the leading 1)
  std::bitset<LENGTH> result_mantissa;
  for (int i = 0; i < LENGTH; ++i) {
    result_mantissa[i] = normalized[i];
  }

  return LongDoubleBitset<LENGTH>(result_mantissa, result_sign, result_exponent, false);
}

template<int LENGTH>
LongDoubleBitset<LENGTH> operator/(const LongDoubleBitset<LENGTH>& lhs, const LongDoubleBitset<LENGTH>& rhs) {
  if (rhs.is_zero) {
    throw std::runtime_error("Division by zero in LongDoubleBitset");
  }
  if (lhs.is_zero) {
      return LongDoubleBitset<LENGTH>(
          std::bitset<LENGTH>(0), 
          lhs.signbit ^ rhs.signbit,
          0,
          true
      );
  }

  constexpr int FULL = LENGTH + 2;
  constexpr int DIV_BITS = 2 * FULL;

  // 1. Get mantissas with leading 1
  std::bitset<FULL> num = lhs.get_full_mantissa();
  std::bitset<FULL> den = rhs.get_full_mantissa();

  // 2. Left-align numerator to DIV_BITS
  std::bitset<DIV_BITS> dividend(0);
  for (int i = 0; i < FULL; ++i) {
    dividend[DIV_BITS - 1 - i] = num[FULL - 1 - i];
  }

  std::bitset<DIV_BITS> divisor(0);
  for (int i = 0; i < FULL; ++i) {
    divisor[FULL - 1 - i] = den[FULL - 1 - i];
  }

  // 3. Perform binary long division
  std::bitset<DIV_BITS> quotient(0);
  std::bitset<DIV_BITS> remainder(0);

  for (int i = DIV_BITS - 1; i >= 0; --i) {
    remainder <<= 1;
    remainder[0] = dividend[i];

    if (!bitset_lt<DIV_BITS>(remainder, divisor)) {
      remainder = bitset_subtract<DIV_BITS>(remainder, divisor);
      quotient.set(i);
    }
  }

  // 4. Find MSB of result to normalize
  int msb = -1;
  for (int i = DIV_BITS - 1; i >= 0; --i) {
    if (quotient[i]) {
      msb = i;
      break;
    }
  }

  if (msb == -1) {
    // Result is zero
    return LongDoubleBitset<LENGTH>(
      std::bitset<LENGTH>(0),
      lhs.signbit ^ rhs.signbit,
      0,
      true
    );
  }

  // 5. Normalize
  int shift = msb - (LENGTH);
  std::bitset<DIV_BITS> normalized(0);
  if (shift >= 0) {
    normalized = quotient >> shift;
  } else {
    normalized = quotient << -shift;
  }

  // 6. Extract mantissa
  std::bitset<LENGTH> result_mantissa(0);
  for (int i = 0; i < LENGTH; ++i) {
    result_mantissa[i] = normalized[i];
  }

  // 7. Compute exponent
  int result_exponent = lhs.exponent - rhs.exponent + (shift - 1);

  // 8. Return result
  return LongDoubleBitset<LENGTH>(
      result_mantissa,
      lhs.signbit ^ rhs.signbit,
      result_exponent,
      false
  );
}
