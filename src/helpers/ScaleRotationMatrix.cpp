#include "ScaleRotationMatrix.h"

template<typename T>
ScaleRotationMatrix<T>::ScaleRotationMatrix(const Line<T>& line1, const Line<T>& line_2) {
  T aX = line1.b.x - line1.a.x;
  T aY = line1.b.y - line1.a.y;
  T bX = line_2.b.x - line_2.a.x;
  T bY = line_2.b.y - line_2.a.y;
  scaledCos = ( (aX * bX) + (aY * bY) ) / line1.get_linear_size_squared();
  scaledSin = ( (aX * bY) - (aY * bX) ) / line1.get_linear_size_squared();
}

template<typename T>
ScaleRotationMatrix<T>::ScaleRotationMatrix() {}

template<typename T>
Line<T> ScaleRotationMatrix<T>::MultiplyByVector(const Line<T>& vector) const {
  T vecX = vector.b.x - vector.a.x;
  T vecY = vector.b.y - vector.a.y;
  T outVecX = vecX * scaledCos - vecY * scaledSin;
  T outVecY = vecX * scaledSin + vecY * scaledCos;

  Line<T> outVec = Line<T>(Position<T>(0.0, 0.0), Position<T>(outVecX, outVecY));
  return outVec;
}
