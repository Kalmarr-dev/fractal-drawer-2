#pragma once

class ILongDouble
{
public:
  virtual ~ILongDouble();

  virtual double get_double(const ILongDouble& offset, int scale_exponent) = 0;
};