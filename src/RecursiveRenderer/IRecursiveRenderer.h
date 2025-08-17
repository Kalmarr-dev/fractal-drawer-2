#pragma once

#include "../helpers/Position.h"

template<typename T>
class IRecursiveRenderer
{
public:
  virtual ~IRecursiveRenderer();
  virtual Shapes get_shapes_on_camera(Position<T> lower, Position<T> higher) = 0;
};