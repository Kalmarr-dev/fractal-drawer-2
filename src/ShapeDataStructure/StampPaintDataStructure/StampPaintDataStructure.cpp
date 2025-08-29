#include "StampPaintDataStructure.h"

#include <math.h>
#include <iostream>

template<typename T>
StampPaintDataStructure<T>::StampPaintDataStructure(ICamera<T>* p_camera, Configuration configuration)
  : p_camera(p_camera), configuration(configuration) 
{
  this->previous_camera_scale = p_camera->get_bigger_side();
  this->current_screen_coordinate_squares = nullptr;
}
  
template<typename T>
void StampPaintDataStructure<T>::update_temporary_shapes() {
  for (auto &&shape : temporary_shapes.get_shapes())
  {
    delete shape;
  }
  temporary_shapes.clear();
  if (this->current_screen_coordinate_squares != nullptr)
  {
    Figure<T>* figure = new Figure<T>(this->current_screen_coordinate_squares, T(0.0));
    for (auto &&i : figure->get_shapes().get_shapes())
    {
      temporary_shapes.add_shape(i);
    }
    delete figure;
  }
  // TODO add current stamp
}

template<typename T>
void StampPaintDataStructure<T>::update_temporary_shapes(const Shapes<T> shapes) {
  for (auto &&shape : temporary_shapes.get_shapes())
  {
    delete shape;
  }
  temporary_shapes.clear();
  for (auto &&i : shapes.get_shapes())
  {
    temporary_shapes.add_shape(i);
  }
}

template<typename T>
void StampPaintDataStructure<T>::clear_shapes() {
  throw "Not implemented";
}

template<typename T>
Shapes<T> StampPaintDataStructure<T>::clear_last_shapes() {
  Shapes<T> shapes;
  if (figure_was_drawn.size() > 0)
  {
    if (figure_was_drawn.back())
    {
      Figure<T>* figure = all_figures.back();
      all_figures.pop_back();
      shapes = figure->get_shapes();
      figure->clear();
    } else {
      // Stamp<T>* stamp = all_stamps.back();
      // all_stamps.pop_back();
      // shapes = stamp->get_shapes();
      // stamp->clear();
    }
    figure_was_drawn.pop_back();
  }
  return shapes;
}

template<typename T>
Shapes<T> StampPaintDataStructure<T>::get_temporary_shapes() {
  return temporary_shapes;
}

template<typename T>
Shapes<T> StampPaintDataStructure<T>::get_new_shapes() {
  auto shapes = new_shapes;
  new_shapes.clear();
  return shapes;
}

template<typename T>
void StampPaintDataStructure<T>::process_pointer_move(Position<T> pointer) {
  if (current_screen_coordinate_squares != nullptr)
  {
    T camera_size = p_camera->get_bigger_side();
    auto corners = p_camera->get_camera_corners();
    T width = corners.second.x - corners.first.x;
    T height = corners.second.y - corners.first.y;
    T x = corners.first.x + width * pointer.x;
    T y = corners.first.y + height * pointer.y;
    current_screen_coordinate_squares->add_circle_of_squares({x, y}, T(this->brush_radius) * camera_size);
  }
  update_temporary_shapes();
}

template<typename T>
void StampPaintDataStructure<T>::process_primary_click(Position<T> pointer) {
  T camera_size = p_camera->get_bigger_side();
  auto corners = p_camera->get_camera_corners();
  T width = corners.second.x - corners.first.x;
  T height = corners.second.y - corners.first.y;
  T x = corners.first.x + width * pointer.x;
  T y = corners.first.y + height * pointer.y;
  current_screen_coordinate_squares = new ScreenCoordinateSquares<T>({x, y}, T(this->square_size) * camera_size);
  current_screen_coordinate_squares->add_circle_of_squares({x, y}, T(this->brush_radius) * camera_size);
  update_temporary_shapes();
}

template<typename T>
void StampPaintDataStructure<T>::process_pointer_up() {
  if (current_screen_coordinate_squares)
  {
    all_figures.push_back(
      new Figure<T>(
        current_screen_coordinate_squares,
        T(std::pow(0.5, last_depth_exponent_numerator / depth_exponent_denominator + 1))
      )
    );
    figure_was_drawn.push_back(true);
    last_depth_exponent_numerator++;
    for (auto &&shape : all_figures.back()->get_shapes().get_shapes())
    {
      new_shapes.add_shape(shape);
    }
    delete current_screen_coordinate_squares;
    current_screen_coordinate_squares = nullptr;
  }
  // TODO delete current stamp too
  update_temporary_shapes(Shapes<T>());
}

template<typename T>
void StampPaintDataStructure<T>::process_secondary_click(Position<T> pointer) {

}

template<typename T>
void StampPaintDataStructure<T>::process_zoom(Position<T> pointer, T scale_value) {
  if (p_camera->get_bigger_side() * this->camera_scale_change_to_recalculate_stamps < this->previous_camera_scale
  || this->previous_camera_scale * this->camera_scale_change_to_recalculate_stamps < p_camera->get_bigger_side())
  {
    this->previous_camera_scale = p_camera->get_bigger_side();
  }
}
