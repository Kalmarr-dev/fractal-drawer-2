#pragma once

#include "../LongDouble/ILongDouble.h"
#include "IShape.fwd.h"
#include "Rectangle.h"
#include <utility>

template<typename T>
class IShape
{
public:
  virtual ~IShape() = default;
  virtual bool collides_with(const Rectangle<T>& rectangle) = 0;
  virtual inline T get_linear_size_squared() = 0;
  virtual inline std::vector<Position<T>> get_points() = 0;
  virtual inline std::vector<unsigned int> get_indexes() = 0;
};
