#include "Figure.h"

#include "../../../Shapes/Rectangle.h"

template<typename T>
Figure<T>::Figure(ScreenCoordinateSquares<T>* screen_coordinate_squares, T depth) {
  for (auto &&coordinates : screen_coordinate_squares->square_coordinates)
  {
    T x1 = T(coordinates.first) * screen_coordinate_squares->square_size + screen_coordinate_squares->origin.x;
    T x2 = x1 + screen_coordinate_squares->square_size;
    T y1 = T(coordinates.second) * screen_coordinate_squares->square_size + screen_coordinate_squares->origin.y;
    T y2 = y1 + screen_coordinate_squares->square_size;
    Rectangle<T>* rectangle = new Rectangle(Position<T>(x1, y1), Position<T>(x2, y2), depth);
    shapes.add_shape(rectangle);
  }
}

template<typename T>
Shapes<T> Figure<T>::get_shapes() {
  return shapes;
}

template<typename T>
void Figure<T>::clear() {
  for (auto &&i : shapes.get_shapes())
  {
    delete i;
  }
  shapes.clear();
}