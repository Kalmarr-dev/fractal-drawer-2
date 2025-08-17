#pragma once

#include <vector>
#include "../LongDouble/ILongDouble.h"
#include "IShape.h"

template<typename T>
class Shapes
{
private:
  std::vector<IShape<T>> shapes;

public:
  void add_shape(IShape<T> shape) {
    shapes.push_back(shape);
  }

  std::vector<IShape<T>> get_shapes() {
    return shapes;
  }

  

  ~Shapes();
};

