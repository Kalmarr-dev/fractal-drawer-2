#include "BasicSquareOnScreenButton.h"

BasicSquareOnScreenButton::BasicSquareOnScreenButton(Position<double> offset, double width, Shapes<double> shapes, Color<double> color)
{
  this->offset = offset;
  this->width = width;
  this->height = width;
  this->shapes = shapes;
  this->color = color;
}

Shapes<double> BasicSquareOnScreenButton::get_shapes(double window_width, double window_height) {
  Shapes<double> scaled_shapes;
  for (auto &&i : shapes.get_shapes())
  {
    scaled_shapes.add_shape(i->scale(1.0 / window_width, 1.0 / window_height)->add_one_to_negative_coordinates());
  }
  return scaled_shapes;
}
