#include "FractalDataStructure.h"

#include "../../Shapes/Line.h"

template<typename T>
FractalDataStructure<T>::FractalDataStructure() {}

template<typename T>
void FractalDataStructure<T>::clear_shapes() {
  currently_drawn_fractal.clear();
  // all_fractals.clear();
  all_shapes.clear();
  new_shapes.clear();
}

template<typename T>
void FractalDataStructure<T>::update_shapes_on_zoom(ICamera<T>* p_camera) {
  // TODO
}

template<typename T>
Shapes<T> FractalDataStructure<T>::get_new_shapes() {
  auto shapes = new_shapes;
  new_shapes.clear();
  return shapes;
}

template<typename T>
void FractalDataStructure<T>::process_primary_click(Position<T> pointer) {
  all_shapes.add_shape(new Line<T>(pointer, Position<T>{pointer.x, pointer.y + 50}));
  new_shapes.add_shape(new Line<T>(pointer, Position<T>{pointer.x, pointer.y + 50}));
}

template<typename T>
void FractalDataStructure<T>::process_secondary_click(Position<T> pointer) {
  all_shapes.add_shape(new Line<T>(pointer, Position<T>{pointer.x, pointer.y - 50}));
  new_shapes.add_shape(new Line<T>(pointer, Position<T>{pointer.x, pointer.y - 50}));
}
