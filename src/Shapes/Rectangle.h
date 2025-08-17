#pragma once

#include "../LongDouble/ILongDouble.h"
#include "IShape.h"

template<typename T>
class Rectangle : public IShape<T>
{
public:
  ~Rectangle() = default;
  bool collides_with(const Rectangle<T>& rectangle) {
    throw "not implemented";
  }
};

