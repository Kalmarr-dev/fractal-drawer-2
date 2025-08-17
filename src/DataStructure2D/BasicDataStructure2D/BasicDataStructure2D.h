#pragma once

#include "../IDataStructure2D.h"
#include "../../Shapes/Shapes.h"

template<typename T>
class BasicDataStructure2D : IDataStructure2D<T>
{
private:
  Shapes<T> shapes;
public:
  ~BasicDataStructure2D();

  void add_shapes(Shapes<T> shapes) override;
  Shapes<T> get_visible_shapes_in_area(Position<T> lower, Position<T> higher) override;
};
