#include "BasicLongDouble.h"

BasicLongDouble::BasicLongDouble(double value) {
  this->value = value;
}

double BasicLongDouble::get_double(ILongDouble* offset, int scale_exponent) {
  return (this->value - dynamic_cast<BasicLongDouble*>(offset)->value);
}
  
bool operator<(const BasicLongDouble& lhs, const BasicLongDouble& rhs) {
  return lhs.value < rhs.value;
}
BasicLongDouble operator+(const BasicLongDouble& lhs, const BasicLongDouble& rhs) {
  return lhs.value + rhs.value;
}
BasicLongDouble operator-(const BasicLongDouble& lhs, const BasicLongDouble& rhs) {
  return lhs.value - rhs.value;
}
BasicLongDouble operator*(const BasicLongDouble& lhs, const BasicLongDouble& rhs) {
  return lhs.value * rhs.value;
}
BasicLongDouble operator/(const BasicLongDouble& lhs, const BasicLongDouble& rhs) {
  return lhs.value / rhs.value;
}