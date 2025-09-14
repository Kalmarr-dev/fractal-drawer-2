#include "BasicLongDouble.h"

BasicLongDouble::BasicLongDouble(double value) {
  this->value = value;
}
  
bool operator<(const BasicLongDouble& lhs, const BasicLongDouble& rhs) {
  return lhs.value < rhs.value;
}
bool operator==(const BasicLongDouble& lhs, const BasicLongDouble& rhs) {
  return lhs.value == rhs.value;
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