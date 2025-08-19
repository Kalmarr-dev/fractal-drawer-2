#include "ScaleRotationMatrix.h"

template<typename T>
ScaleRotationMatrix<T>::ScaleRotationMatrix(const Line<T>& line1, const Line<T>& line_2) {
  double aX = line1.b.x - line1.a.x;
  double aY = line1.b.y - line1.a.y;
  double bX = line_2.b.x - line_2.a.x;
  double bY = line_2.b.y - line_2.a.y;
  scaledCos = ( (aX * bX) + (aY * bY) ) / line1.get_linear_size_squared();
  scaledSin = ( (aX * bY) - (aY * bX) ) / line1.get_linear_size_squared();
}

template<typename T>
ScaleRotationMatrix<T>::ScaleRotationMatrix() {}

template<typename T>
Line<T> ScaleRotationMatrix<T>::MultiplyByVector(const Line<T>& vector) const {
  double vecX = vector.b.x - vector.a.x;
  double vecY = vector.b.y - vector.a.y;
  double outVecX = vecX * scaledCos - vecY * scaledSin;
  double outVecY = vecX * scaledSin + vecY * scaledCos;

  Line<T> outVec = {0.0, 0.0, outVecX, outVecY};
  return outVec;
}
