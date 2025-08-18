#include "BasicLongDouble.h"

BasicLongDouble::BasicLongDouble(double value) {
  this->value = value;
}

double BasicLongDouble::get_double(ILongDouble* offset, int scale_exponent) {
  return (this->value - dynamic_cast<BasicLongDouble*>(offset)->value);
}
  
bool BasicLongDouble::operator<(const BasicLongDouble& rhs) {
  return this->value < rhs.value;
}
BasicLongDouble BasicLongDouble::operator+(const BasicLongDouble& rhs) {
  return this->value + rhs.value;
}
BasicLongDouble BasicLongDouble::operator-(const BasicLongDouble& rhs) {
  return this->value - rhs.value;
}
BasicLongDouble BasicLongDouble::operator*(const BasicLongDouble& rhs) {
  return this->value * rhs.value;
}
BasicLongDouble BasicLongDouble::operator/(const BasicLongDouble& rhs) {
  return this->value / rhs.value;
}