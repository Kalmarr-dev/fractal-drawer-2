#pragma once

#include <vector>
#include "../ILongDouble.h"

class LongDoubleVector : public ILongDouble
{
private:
  std::vector<double> values;
  int exponent;
public:
  LongDoubleVector();
  ~LongDoubleVector();

  // casts ILongDouble to LongDoubleVector
  double get_double(const ILongDouble& offset, int scale_exponent);
  
  LongDoubleVector operator<(const LongDoubleVector& rhs);

  LongDoubleVector operator+(const LongDoubleVector& rhs);
  LongDoubleVector operator-(const LongDoubleVector& rhs);
  
  // These can not be optimized like that
  // LongDoubleVector operator*(const LongDoubleVector& rhs);
  // LongDoubleVector operator/(const LongDoubleVector& rhs);
};