#pragma once

#include "../LongDouble/ILongDouble.h"
#include "../Shapes/Shapes.h"
#include "../helpers/Position.h"

template<typename T>
class IDataStructure2D
{
public:
  virtual ~IDataStructure2D();

  virtual void add_shapes(Shapes<T> shapes) = 0;
  virtual Shapes<T> get_visible_shapes_in_area(Position<T> lower, Position<T> higher) = 0;
};
