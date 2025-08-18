#pragma once

#include "../LongDouble/ILongDouble.h"
#include "../helpers/Position.h"
#include "IShape.h"

template<typename T>
class Rectangle : public IShape<T>
{
private:
  Position<T> lower;
  Position<T> higher;
public:
  Rectangle(Position<T> a, Position<T> b) {
    if (b.x < a.x)
    {
      std::swap(a.x, b.x);
    }
    if (b.y < a.y)
    {
      std::swap(a.y, b.y);
    }
    this->lower = a;
    this->higher = b;
  }

  ~Rectangle() = default;

  bool collides_with(const Rectangle<T>& rectangle) {
    throw "not implemented";
  }

  inline T get_linear_size_squared() {
    return (higher.x - lower.x) * (higher.x - lower.x) + (higher.y - lower.y) * (higher.y - lower.y);
  }
};

