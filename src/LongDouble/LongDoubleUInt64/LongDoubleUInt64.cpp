#include "LongDoubleUInt64.h"

#include <math.h>
#include <stdexcept>

template<int LENGTH>
LongDoubleUInt64<LENGTH>::LongDoubleUInt64(double value) {
  this->values = std::array<uint64_t, LENGTH>();
  for (size_t i = 0; i < LENGTH; i++)
  {
    this->values[i] = 0;
  }
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
    mantissa = scalb(mantissa, std::numeric_limits<double>::digits);
    this->values[0] = mantissa;
    this->values[0] <<= 64 - std::numeric_limits<double>::digits;
  } else {
    this->is_zero = true;
  }
}

template<int LENGTH>
LongDoubleUInt64<LENGTH>::LongDoubleUInt64(const std::array<uint64_t, LENGTH>& values, bool signbit, int exponent, bool is_zero)
  : values(values), signbit(signbit), exponent(exponent), is_zero(is_zero) {}

template<int LENGTH>
double LongDoubleUInt64<LENGTH>::get_double(ILongDouble* offset, int scale_exponent) {
  LongDoubleUInt64<LENGTH>* offset_cast = dynamic_cast<LongDoubleUInt64<LENGTH>*>(offset);
  
  uint64_t result_uint64_t = 0;
  bool result_signbit = 0;
  if (this->signbit == offset_cast->signbit)
  {
    result_signbit = this->signbit;
    result_uint64_t = (values[0] >> (64 - std::numeric_limits<double>::digits))
                      + (offset_cast->values[0] >> (64 - std::numeric_limits<double>::digits));
  } else {
    if (values[0] > offset_cast->values[0])
    {
      result_signbit = this->signbit;
      result_uint64_t = (values[0] >> (64 - std::numeric_limits<double>::digits))
                        - (offset_cast->values[0] >> (64 - std::numeric_limits<double>::digits));
    } else {
      result_signbit = offset_cast->signbit;
      result_uint64_t = (offset_cast->values[0] >> (64 - std::numeric_limits<double>::digits))
                        - (values[0] >> (64 - std::numeric_limits<double>::digits));
    }
  }

  double mantissa = result_uint64_t;
  mantissa /= uint64_t(1) << std::numeric_limits<double>::digits;
  // Add the implicit leading 1 (normalized numbers)
  if (!this->is_zero)
  {
    mantissa += 1.0;
  }

  double result = std::ldexp(mantissa, exponent + scale_exponent - 1);  // frexp gives mantissa in [0.5, 1), so exponent is biased by +1

  if (result_signbit)
      result = -result;

  return result;
}

template<int LENGTH>
bool operator<(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs) {
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
    bool values_are_equal = true;
    for (size_t i = 0; i < LENGTH; i++)
    {
      if (values_are_equal && lhs.values[i] != rhs.values[i])
      {
        values_are_equal = false;
        abs_less_than = lhs.values[i] < rhs.values[i];
      }
    }

    // If identical, not less than
    if (values_are_equal) {
        return false;
    }
  }

  return lhs.signbit ? !abs_less_than : abs_less_than;
}

template<int LENGTH>
bool operator==(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs) {
  if (lhs.signbit != rhs.signbit) {
    return false;
  }

  if (lhs.is_zero && rhs.is_zero) return true;

  // Both positive or both negative
  return lhs.exponent == rhs.exponent && lhs.values == rhs.values;
}

template<int LENGTH>
LongDoubleUInt64<LENGTH> operator+(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs) {
  if (lhs.is_zero) return rhs;
  if (rhs.is_zero) return lhs;

  const LongDoubleUInt64<LENGTH>* a_aligned;
  const LongDoubleUInt64<LENGTH>* b_aligned;
  int exp_diff = lhs.exponent - rhs.exponent;

  if (exp_diff >= 0) {
    a_aligned = &lhs;
    b_aligned = &rhs;
  } else {
    a_aligned = &rhs;
    b_aligned = &lhs;
    exp_diff = -exp_diff;
  }

  std::array<uint64_t, LENGTH + 1> a_aligned_mant;
  a_aligned_mant[0] = uint64_t(1);
  for (int i = 0; i < LENGTH; ++i) {
    a_aligned_mant[i + 1] = a_aligned->values[i];
  }
  std::array<uint64_t, LENGTH + 1> b_full_mant = {0};
  b_full_mant[0] = uint64_t(1);
  for (int i = 0; i < LENGTH; ++i) {
    b_full_mant[i + 1] = b_aligned->values[i];
  }
  std::array<uint64_t, LENGTH + 1> b_aligned_mant = {0};

  if (exp_diff < (LENGTH + 1) * 64) {
    int whole_words = exp_diff / 64;
    int bit_shift = exp_diff % 64;
    for (int i = LENGTH - 1; i >= whole_words; --i) {
      b_aligned_mant[i] = b_full_mant[i - whole_words] >> bit_shift;
      if (i - whole_words - 1 >= 0 && bit_shift != 0) {
        b_aligned_mant[i] |= b_full_mant[i - whole_words - 1] << (64 - bit_shift);
      }
    }
  }

  std::array<uint64_t, LENGTH + 1> result = {0};
  bool result_sign = false;

  if (lhs.signbit == rhs.signbit) {
    // Same sign: perform addition
    uint64_t carry = 0;
    for (int i = LENGTH + 1 - 1; i >= 0; --i) {
      __uint128_t temp = (__uint128_t)a_aligned_mant[i] + b_aligned_mant[i] + carry;
      result[i] = static_cast<uint64_t>(temp);
      carry = static_cast<uint64_t>(temp >> 64);
    }
    result_sign = lhs.signbit;
  } else {
    // Opposite sign: perform subtraction
    bool a_aligned_mant_is_larger = false;
    for (int i = 0; i < LENGTH + 1; ++i) {
      if (a_aligned_mant[i] != b_aligned_mant[i]) {
          a_aligned_mant_is_larger = a_aligned_mant[i] > b_aligned_mant[i];
          break;
      }
    }

    if (a_aligned_mant_is_larger) {
      // a_aligned - b_aligned
      uint64_t borrow = 0;
      for (int i = LENGTH + 1 - 1; i >= 0; --i) {
          __int128_t temp = (__int128_t)a_aligned_mant[i] - b_aligned_mant[i] - borrow;
          if (temp < 0) {
              temp += (__int128_t(1) << 64);
              borrow = 1;
          } else {
              borrow = 0;
          }
          result[i] = static_cast<uint64_t>(temp);
      }
      result_sign = a_aligned->signbit;
    } else {
      // b_aligned - a_aligned
      uint64_t borrow = 0;
      for (int i = LENGTH + 1 - 1; i >= 0; --i) {
          __int128_t temp = (__int128_t)b_aligned_mant[i] - a_aligned_mant[i] - borrow;
          if (temp < 0) {
              temp += (__int128_t(1) << 64);
              borrow = 1;
          } else {
              borrow = 0;
          }
          result[i] = static_cast<uint64_t>(temp);
      }
      result_sign = b_aligned->signbit;
    }
  }

  // Normalize the result
  int total_shift = 0;
  while (total_shift < (LENGTH + 1) * 64) {
    if ((result[total_shift / 64] >> (63 - total_shift)) != 0)
    {
      break;
    }
    ++total_shift;
  }
  total_shift -= 63;

  if (total_shift > 0) {
    int word_shift = total_shift / 64;
    int bit_shift = total_shift % 64;
    for (int i = 0; i < LENGTH - word_shift; ++i) {
      result[i] = result[i + word_shift] << bit_shift;
      if (i + word_shift + 1 < LENGTH && bit_shift != 0) {
        result[i] |= result[i + word_shift + 1] >> (64 - bit_shift);
      }
    }
    for (int i = LENGTH - word_shift; i < LENGTH; ++i) {
      result[i] = 0;
    }
  } else if (total_shift < 0) {
    int right_shift = -total_shift;
    int word_shift = right_shift / 64;
    int bit_shift = right_shift % 64;
    for (int i = LENGTH + 1 - 1; i >= word_shift; --i) {
      result[i] = result[i - word_shift] >> bit_shift;
      if (i - word_shift - 1 >= 0 && bit_shift != 0) {
        result[i] |= result[i - word_shift - 1] << (64 - bit_shift);
      }
    }
    for (int i = 0; i < word_shift; ++i) {
      result[i] = 0;
    }
  }

  bool is_zero_result = true;
  for (uint64_t v : result) {
    if (v != 0) {
      is_zero_result = false;
      break;
    }
  }

  std::array<uint64_t, LENGTH> result_of_needed_length = {0};
  for (int i = 0; i < LENGTH; ++i) {
    result_of_needed_length[i] = result[i + 1];
  }

  int result_exp = a_aligned->exponent - total_shift;

  return LongDoubleUInt64<LENGTH>(result_of_needed_length, result_sign, result_exp, is_zero_result);
}

template<int LENGTH>
LongDoubleUInt64<LENGTH> operator-(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs) {
  auto new_rhs = LongDoubleUInt64<LENGTH>(rhs.values, !rhs.signbit, rhs.exponent, rhs.is_zero);
  return lhs + new_rhs;
}

template<int LENGTH> 
LongDoubleUInt64<LENGTH> operator*(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs) {
  if (lhs.is_zero || rhs.is_zero) {
    return LongDoubleUInt64<LENGTH>({0},
                                    lhs.signbit ^ rhs.signbit,
                                    0, true);
  }

  bool result_sign = lhs.signbit ^ rhs.signbit;

  int exp_sum = lhs.exponent + rhs.exponent - 1;

  // 4. Build full mantissas (with implicit leading 1)
  const int FULL = LENGTH + 1;
  std::array<uint64_t, FULL> m1 = {0}, m2 = {0};

  m1[0] = UINT64_C(1);
  m2[0] = UINT64_C(1);

  for (int i = 0; i < LENGTH; ++i) {
      m1[i + 1] = lhs.values[i];
      m2[i + 1] = rhs.values[i];
  }

  // 5. Multiply mantissas using schoolbook O(N^2)
  std::array<uint64_t, FULL * 2> prod = {0};

  for (int i = FULL - 1; i >= 0; --i) {
    unsigned __int128 carry = 0;
    for (int j = FULL - 1; j >= 0; --j) {
      unsigned __int128 sum = (unsigned __int128)m1[i] * m2[j] + prod[i + j] + carry;
      prod[i + j] = (uint64_t)sum;
      carry = sum >> 64;
    }
    if (i > 0)
    {
      unsigned __int128 sum = (unsigned __int128)prod[i - 1] + carry;
      prod[i - 1] = (uint64_t)sum;
      unsigned __int128 carry2 = sum >> 64;
      if (carry2 > 0)
      {
        for (int k = i + FULL + 1; k < FULL * 2; k++) {
          sum = (unsigned __int128)prod[k] + carry2;
          prod[k] = (uint64_t)sum;
          carry2 = sum >> 64;
        }
      }
    }
    
  }

  // 6. Normalize the result (find top bit at index)
  int total_words = FULL * 2;
  int top_index = 0;
  while (top_index < total_words && prod[top_index] == 0) {
    ++top_index;
  }

  
  // 7. Shift left to normalize mantissa
  std::array<uint64_t, FULL> normalized = {0};
  
  int leading_zeros = __builtin_clzll(prod[top_index]);
  int shift = leading_zeros - (64 - 1);  // target MSB at bit 63
  // int word_shift = 0;
  int bit_shift = 0;
  if (shift > 0) {
    // word_shift = shift / 64;
    // bit_shift = shift % 64;
    // int src_index = top_index;
    // for (int dest_index = 0; dest_index < FULL && (size_t)src_index < prod.size(); dest_index++)
    // {
    //   unsigned __int128 x = (unsigned __int128)prod[src_index] << bit_shift;
    //   if (bit_shift != 0 && (size_t)src_index + 1 < prod.size()) {
    //     x |= (unsigned __int128)prod[src_index + 1] >> (64 - bit_shift);
    //   }
    //   normalized[dest_index] = (uint64_t)x;
    //   ++src_index;
    // }
    throw std::domain_error("Why is it here? Multiplying two mantissas returned a smaller number");
  } else if (shift <= 0) {
    bit_shift = -shift % 64;
    int src_index = top_index;
    for (int dest_index = 0; dest_index < FULL && (size_t)src_index < prod.size(); dest_index++)
    {
      unsigned __int128 x = (unsigned __int128)prod[src_index] >> bit_shift;
      if (bit_shift != 0 && src_index - 1 >= 0) {
        x |= (unsigned __int128)prod[src_index - 1] << (64 - bit_shift);
      }
      normalized[dest_index] = (uint64_t)x;
      ++src_index;
    }
  }

  exp_sum += (top_index * 64 + (63 - leading_zeros));

  std::array<uint64_t, LENGTH> final_mant;
  for (int i = 0; i < LENGTH; ++i) {
      final_mant[i] = normalized[i + 1];
  }

  return LongDoubleUInt64<LENGTH>(final_mant, result_sign, int(exp_sum), false);
}

template<int LENGTH>
LongDoubleUInt64<LENGTH> operator/(const LongDoubleUInt64<LENGTH>& lhs, const LongDoubleUInt64<LENGTH>& rhs) {
  // 1. Handle special cases
  if (rhs.is_zero) {
    throw std::domain_error("Division by zero");
  }
  if (lhs.is_zero) {
    return LongDoubleUInt64<LENGTH>({0}, false, 0, true);
  }

  // 2. Compute result sign
  bool result_sign = lhs.signbit ^ rhs.signbit;

  // 3. Exponent subtraction; normalize later
  int exp_diff = lhs.exponent - rhs.exponent + 1;

  // 4. Build full normalized mantissas (implicit leading 1)
  const int FULL = LENGTH + 1;
  const int DOUBLE_FULL = FULL * 2;
  std::array<uint64_t, FULL> m1 = {0}, m2 = {0};
  m1[0] = UINT64_C(1);
  m2[0] = UINT64_C(1);
  for (int i = 0; i < LENGTH; ++i) {
      m1[i + 1] = lhs.values[i];
      m2[i + 1] = rhs.values[i];
  }

  // 5. Align dividend for division
  // Find shift so divisor's MSB is at bit 63
  int vs_clz = __builtin_clzll(m2[0]);
  int normalize_shift = vs_clz;

  std::array<uint64_t, FULL> dividend = {0};
  std::array<uint64_t, FULL> divisor = {0};

  if (normalize_shift == 0) {
    dividend = m1;
    divisor = m2;
  } else {
    int bit_shift = normalize_shift;
    int rev_shift = 64 - bit_shift;
    for (int i = 0; i < FULL; ++i) {
      dividend[i] = (m1[i] << bit_shift) | ((i + 1 < FULL) ? (m1[i + 1] >> rev_shift) : 0);
    }
    for (int i = 0; i < FULL; ++i) {
      divisor[i] = (m2[i] << bit_shift) | ((i + 1 < FULL) ? (m2[i + 1] >> rev_shift) : 0);
    }
  }

  // Prepare quotient and remainder words
  std::array<uint64_t, FULL> quotient = {0};
  std::array<uint64_t, DOUBLE_FULL> rem = {0};
  for (size_t i = 0; i < FULL; i++) {
    rem[i] = dividend[i];
  }
  
  // Long division: quotient word-by-word
  const unsigned __int128 base = UINT64_MAX;
  int m = DOUBLE_FULL, n = FULL;
  for (int j = 0; j < m - n; j++) {       // Main loop.
    // Compute estimate qhat of q[j].
    unsigned __int128 qhat = (/*rem[j - 1]*base +*/ rem[j]) / divisor[0];
    unsigned __int128 rhat = (/*rem[j - 1]*base +*/ rem[j]) - qhat * divisor[0];
    if (j > 0)
    {
      qhat = (rem[j - 1]*base + rem[j]) / divisor[0];
      rhat = (rem[j - 1]*base + rem[j]) - qhat * divisor[0];
    }
again:
    if (qhat >= base || qhat*divisor[1] > base*rhat + rem[j+1])
    {
      qhat = qhat - 1;
      rhat = rhat + divisor[0];
      if (rhat < base) goto again;
    }

    // Multiply and subtract.
    unsigned __int128 k = 0;
    for (int i = n - 1; i >= 0; i--) {
        unsigned __int128 p = qhat * divisor[i];
        __int128 t = rem[i+j] - k - (p & UINT64_MAX);
        rem[i+j] = t;
        k = (p >> 64) - (t >> 64);
    }
    __int128 t = 0 - k;
    if (j - 1 > 0) {
      t = rem[j - 1] - k; 
      rem[j - 1] = t;
    }

    quotient[j] = qhat;              // Store quotient digit.
    if (t < 0) {              // If we subtracted too
      quotient[j] = quotient[j] - 1;       // much, add back.
      k = 0;
      for (int i = 0; i < n; i++) {
        t = (unsigned __int128)rem[i+j] + divisor[i] + k;
        rem[i+j] = t;
        k = t >> 64;
      }
      rem[j+n] = rem[j+n] + k;
    }
  } // End j.

  // 6. Normalize quotient mantissa, find msb at bit 63
  int q_top = 0;
  while (q_top < FULL && quotient[q_top] == 0) ++q_top;
  if (q_top == FULL) {
    return LongDoubleUInt64<LENGTH>({0}, result_sign, 0, true);
  }

  int leading = 63 - __builtin_clzll(quotient[q_top]);
  int bit_shift = leading;
  int rev_shift = 64 - bit_shift;
  std::array<uint64_t, FULL> norm_quot = {0};
  if (bit_shift == 0) {
    for (int i = q_top; i < FULL; ++i) {
      norm_quot[i - q_top] = quotient[i];
    }
  } else {
    for (int i = q_top; i < FULL; ++i) {
      norm_quot[i - q_top] = (quotient[i] >> bit_shift) 
                              | (i - 1 < 0 ? 0 : (quotient[i - 1] << rev_shift));
    }
    // Last word only shifted (no higher word to pull from)
    if (q_top > 0)
    {
      norm_quot[FULL - q_top] = quotient[FULL - 1] << rev_shift;
    }
  }

  // exp_diff += normalize_shift;             // Undo initial normalization
  exp_diff -= (q_top * 64 - leading);      // Account for shifting MSB to bit63

  // 7. Build final mantissa (drop implicit 1)
  std::array<uint64_t, LENGTH> final_mant;
  for (int i = 0; i < LENGTH; ++i) {
    final_mant[i] = norm_quot[i + 1];
  }

  return LongDoubleUInt64<LENGTH>(final_mant, result_sign, (int)exp_diff, false);
}
