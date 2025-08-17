#pragma once

#include "../LongDouble/ILongDouble.h"

template<typename T = double>
class IDataStructure2D
{
public:
  virtual ~I2DDataStructure();

  virtual void add_shape() = 0;
  virtual void get_visible_shapes_in_area(Position<T> lower, Position<T> higher) = 0;
};
