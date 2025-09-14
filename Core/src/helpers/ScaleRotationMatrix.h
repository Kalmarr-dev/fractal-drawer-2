#pragma once

#include "../Shapes/Line.h"

template<typename T>
struct ScaleRotationMatrix {
public:
  T scaledCos;
  T scaledSin;

  ScaleRotationMatrix(const Line<T>& a, const Line<T>& b);
  ScaleRotationMatrix();
  Line<T> MultiplyByVector(const Line<T>& vector) const;
};
