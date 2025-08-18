#pragma once

#include "../LongDouble/ILongDouble.h"
#include "IShape.fwd.h"
#include "Rectangle.h"

template<typename T>
class IShape
{
public:
  virtual ~IShape() = default;
  virtual bool collides_with(const Rectangle<T>& rectangle) = 0;
  virtual inline T get_linear_size_squared() = 0;
  // virtual inline pair<vector<T>, vector<int>> get_points_and_indexes() = 0;
};
