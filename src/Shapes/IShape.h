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
};

