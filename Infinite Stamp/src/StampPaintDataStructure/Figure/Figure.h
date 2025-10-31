#pragma once

#include <Shapes/Shapes.h>
#include "ScreenCoordinateSquares.h"

template<typename T>
class Figure
{
private:
  Shapes<T> shapes;
public:
  Figure(ScreenCoordinateSquares<T>* screen_coordinate_squares, T depth);
  ~Figure() = default;
  Shapes<T> get_shapes();
  void clear();
};
