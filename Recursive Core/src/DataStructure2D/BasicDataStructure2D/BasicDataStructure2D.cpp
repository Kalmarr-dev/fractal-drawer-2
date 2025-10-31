#include "BasicDataStructure2D.h"

template<typename T>
void BasicDataStructure2D<T>::add_shapes(Shapes<T> shapes) {
  for (auto &&i : shapes.get_shapes())
  {
    this->shapes_set.insert(i);
  }
}

template<typename T>
Shapes<T> BasicDataStructure2D<T>::get_visible_shapes_in_area(Position<T> lower, Position<T> higher) {
  Shapes<T> shapes;
  for (auto &&shape : shapes_set)
  {
    shapes.add_shape(shape);
  }
  return shapes;
}

template<typename T>
void BasicDataStructure2D<T>::clear_shapes() {
  this->shapes_set.clear();
}

template<typename T>
void BasicDataStructure2D<T>::clear_shapes(const Shapes<T>& shapes) {
  for (auto &&i : shapes.get_shapes())
  {
    this->shapes_set.erase(i);
  }
}