#pragma once

class ILongDouble
{
public:
  virtual ~ILongDouble() = default;

  virtual double get_double(ILongDouble* offset, int scale_exponent) = 0;
};