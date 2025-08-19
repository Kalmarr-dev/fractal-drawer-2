#pragma once

#include "../../../LongDouble/ILongDouble.h"
#include "../../../Shapes/Line.h"

template<typename T>
class Fractal
{
public:
  ~Fractal() = default;

  Shapes<T> update_on_zoom();
};
