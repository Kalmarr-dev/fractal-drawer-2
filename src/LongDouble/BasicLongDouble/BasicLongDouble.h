#pragma once

#include <vector>
#include "../ILongDouble.h"

class BasicLongDouble : public ILongDouble
{
private:
  double value;
public:
  BasicLongDouble(double value);
  ~BasicLongDouble() = default;

  // casts ILongDouble to BasicLongDouble
  double get_double(const ILongDouble& offset, int scale_exponent);
  
  bool operator<(const BasicLongDouble& rhs);

  BasicLongDouble operator+(const BasicLongDouble& rhs);
  BasicLongDouble operator-(const BasicLongDouble& rhs);
  
  BasicLongDouble operator*(const BasicLongDouble& rhs);
  BasicLongDouble operator/(const BasicLongDouble& rhs);
};