#include "FractalDataStructure.h"

#include "../../Shapes/Line.h"

template<typename T>
FractalDataStructure<T>::FractalDataStructure(ICamera<T>* p_camera) : p_camera(p_camera) {}

template<typename T>
void FractalDataStructure<T>::draw_silly_line(Position<T> pointer) {
  auto corners = p_camera->get_camera_corners();
  T width = corners.second.x - corners.first.x;
  T height = corners.second.y - corners.first.y;
  T x = corners.first.x + width * pointer.x;
  T y = corners.first.y + height * pointer.y;
  all_shapes.add_shape(new Line<T>({x, y}, {x, y + 0.5}));
  new_shapes.add_shape(new Line<T>({x, y}, {x, y + 0.5}));
}

template<typename T>
void FractalDataStructure<T>::clear_shapes() {
  fractal_stub.clear();
  all_fractals.clear();
  all_shapes.clear();
  new_shapes.clear();
}

template<typename T>
void FractalDataStructure<T>::update_shapes_on_zoom() {
  // TODO
}

template<typename T>
Shapes<T> FractalDataStructure<T>::get_temporary_shapes() {
  return temporary_shapes;
}

template<typename T>
Shapes<T> FractalDataStructure<T>::get_new_shapes() {
  auto shapes = new_shapes;
  new_shapes.clear();
  return shapes;
}

template<typename T>
void FractalDataStructure<T>::process_primary_click(Position<T> pointer) {
  // draw_silly_line(pointer);
  auto corners = p_camera->get_camera_corners();
  T width = corners.second.x - corners.first.x;
  T height = corners.second.y - corners.first.y;
  T x = corners.first.x + width * pointer.x;
  T y = corners.first.y + height * pointer.y;
  fractal_stub.add_point_to_root_line({x, y});

  temporary_shapes.clear();

  for (auto &&i : fractal_stub.get_recursed_lines(1).get_shapes())
  {
    temporary_shapes.add_shape(i);
  }
}

template<typename T>
void FractalDataStructure<T>::process_secondary_click(Position<T> pointer) {
  // draw_silly_line(Position<T>{pointer.x, pointer.y - 0.5});
  auto corners = p_camera->get_camera_corners();
  T width = corners.second.x - corners.first.x;
  T height = corners.second.y - corners.first.y;
  T x = corners.first.x + width * pointer.x;
  T y = corners.first.y + height * pointer.y;
  fractal_stub.add_point_to_direction_line({x, y});

  temporary_shapes.clear();

  for (auto &&i : fractal_stub.get_recursed_lines(1).get_shapes())
  {
    temporary_shapes.add_shape(i);
  }
}

template<typename T>
void FractalDataStructure<T>::process_confirm() {
  
}
