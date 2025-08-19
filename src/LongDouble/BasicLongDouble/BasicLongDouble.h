#pragma once

#include <vector>
#include "../ILongDouble.h"

class BasicLongDouble : public ILongDouble
{
public:
  double value;
  BasicLongDouble(double value = 0.0);
  ~BasicLongDouble() = default;

  // casts ILongDouble to BasicLongDouble
  double get_double(ILongDouble* offset, int scale_exponent);
};

bool operator<(const BasicLongDouble& lhs, const BasicLongDouble& rhs);

BasicLongDouble operator+(const BasicLongDouble& lhs, const BasicLongDouble& rhs);
BasicLongDouble operator-(const BasicLongDouble& lhs, const BasicLongDouble& rhs);

BasicLongDouble operator*(const BasicLongDouble& lhs, const BasicLongDouble& rhs);
BasicLongDouble operator/(const BasicLongDouble& lhs, const BasicLongDouble& rhs);