#pragma once

#include "../LongDouble/ILongDouble.h"
#include "IShape.fwd.h"
#include "Rectangle.h"
#include <utility>
#include <vector>

template<typename T>
class IShape
{
public:
  virtual ~IShape() = default;
  virtual bool collides_with(const Rectangle<T>& rectangle) const = 0;
  virtual inline T get_linear_size_squared() const = 0;
  virtual inline std::vector<Position<T>> get_points() const = 0;
  virtual inline std::vector<unsigned int> get_indexes() const = 0;
};
