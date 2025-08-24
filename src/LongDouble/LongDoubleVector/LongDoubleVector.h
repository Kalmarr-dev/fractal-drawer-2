#pragma once

#include <vector>
#include "../ILongDouble.h"

class LongDoubleVector : public ILongDouble
{
private:
public:
  // either all are negative or all are positive
  std::vector<int> values;
  static const int precision_per_int = 10;
  // is divisible by precision_per_int
  int current_exponent;

  LongDoubleVector(double value = 0.0);
  LongDoubleVector(const std::vector<int>& values, int exponent);
  ~LongDoubleVector() = default;

  // casts ILongDouble to LongDoubleVector
  double get_double(ILongDouble* offset, int scale_exponent) override;
  int get_sign() const;
  LongDoubleVector get_normalized() const;
};

bool operator<(const LongDoubleVector& lhs, const LongDoubleVector& rhs);

LongDoubleVector operator+(const LongDoubleVector& lhs, const LongDoubleVector& rhs);
LongDoubleVector operator-(const LongDoubleVector& lhs, const LongDoubleVector& rhs);

LongDoubleVector operator*(const LongDoubleVector& lhs, const LongDoubleVector& rhs);
LongDoubleVector operator/(const LongDoubleVector& lhs, const LongDoubleVector& rhs);