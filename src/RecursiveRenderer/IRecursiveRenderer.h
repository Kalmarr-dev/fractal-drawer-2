#pragma once

#include "../helpers/Position.h"
#include "../Shapes/Shapes.h"

template<typename T>
class IRecursiveRenderer
{
public:
  virtual ~IRecursiveRenderer() = default;
  virtual Shapes<T> get_shapes_on_camera() const = 0;
};