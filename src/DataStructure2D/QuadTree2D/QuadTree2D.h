#pragma once

#include "../IDataStructure2D.h"
#include "../../Shapes/Shapes.h"

template<typename T = double>
class QuadTree2D : IDataStructure2D<T>
{
public:
  virtual ~QuadTree2D();

  virtual void add_shapes(Shapes<T> shapes) = 0;
  virtual Shapes<T> get_visible_shapes_in_area(Position<T> lower, Position<T> higher) = 0;
};
