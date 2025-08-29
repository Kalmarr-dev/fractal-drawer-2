#pragma once

#include "../../../Shapes/Shapes.h"

template<typename T>
class Stamp
{
private:
  Shapes<T> new_shapes;
public:
  Stamp();
  ~Stamp() = default;
  Shapes<T> get_new_shapes();
  void clear();
};
