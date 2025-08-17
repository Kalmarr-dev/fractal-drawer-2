#pragma once

#include "../LongDouble/ILongDouble.h"
#include "IShape.h"

template<typename T = double>
class Rectangle : public IShape<T>
{
public:
  ~Rectangle();
  bool collides_with(const Rectangle<T>& rectangle) {
    throw "not implemented";
  }
};

