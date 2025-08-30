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
  if (this->current_stamp != nullptr) {
    Shapes<T> lines = this->current_stamp->get_lines_recursive();
    for (auto &&i : lines.get_shapes())
    {
      auto p_line = new Line<T>(*dynamic_cast<Line<T>*>(i));
      temporary_shapes.add_shape(p_line);
    }
  }
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
      // figure->clear();
      delete figure;
    } else {
      Stamp<T>* stamp = all_stamps[0];
      if (stamp != nullptr)
      {
        all_stamps[0] = nullptr;
        shapes = stamp->clear_children();
        delete stamp;
      }
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
  if (current_stamp != nullptr)
  {
    T camera_size = p_camera->get_bigger_side();
    auto corners = p_camera->get_camera_corners();
    T width = corners.second.x - corners.first.x;
    T height = corners.second.y - corners.first.y;
    T x = corners.first.x + width * pointer.x;
    T y = corners.first.y + height * pointer.y;
    auto first_corner = this->current_stamp->get_root_line().a;
    this->current_stamp = new Stamp<T>(Line<T>(first_corner, Position<T>(first_corner.x, y)), x - first_corner.x, Shapes<T>());
    this->current_stamp->set_depth(0.0);
    // this->current_stamp->add_child_stamp(
    //   Line<T>(Position<T>(first_corner.x, y), Position<T>(first_corner.x, y - (y - first_corner.y) * T(0.8))),
    //   (x - first_corner.x) * T(0.8)
    // );
    // T minimum_visible_line_size = T(this->configuration.minimum_shape_size) * p_camera->get_bigger_side();
    // this->current_stamp->update_on_zoom(p_camera, configuration.max_number_of_elements_in_memory, minimum_visible_line_size, 10);
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
        T(std::pow(0.5, (*last_depth_exponent_numerator) / StampPaintDataStructure<T>::depth_exponent_denominator + 1))
      )
    );
    figure_was_drawn.push_back(true);
    (*last_depth_exponent_numerator)++;
    for (auto &&shape : all_figures.back()->get_shapes().get_shapes())
    {
      new_shapes.add_shape(shape);
    }
    delete current_screen_coordinate_squares;
    current_screen_coordinate_squares = nullptr;
    for (auto &&stamp : this->all_stamps)
    {
      if (stamp != nullptr)
      {
        for (auto &&shape : stamp->update_on_new_shapes(new_shapes, configuration.max_number_of_elements_rendered).get_shapes())
        {
          new_shapes.add_shape(shape);
        }
      }
    }
  }
  // TODO update current_stamp readiness
  update_temporary_shapes();
}

template<typename T>
void StampPaintDataStructure<T>::process_secondary_click(Position<T> pointer) {
  T camera_size = p_camera->get_bigger_side();
  auto corners = p_camera->get_camera_corners();
  T width = corners.second.x - corners.first.x;
  T height = corners.second.y - corners.first.y;
  T x = corners.first.x + width * pointer.x;
  T y = corners.first.y + height * pointer.y;
  this->current_stamp = new Stamp<T>(Line<T>({x, y}, Position<T>(x, y + T(0.001))), 0.001, Shapes<T>());
  this->current_stamp->set_depth(0.0);
  // T minimum_visible_line_size = T(this->configuration.minimum_shape_size) * p_camera->get_bigger_side();
  // this->current_stamp->update_on_zoom(p_camera, configuration.max_number_of_elements_in_memory, minimum_visible_line_size, 10);
  update_temporary_shapes();
}

template<typename T>
void StampPaintDataStructure<T>::process_zoom(Position<T> pointer, T scale_value) {
  if (p_camera->get_bigger_side() * T(this->configuration.camera_change) < this->previous_camera_scale
  || this->previous_camera_scale * T(this->configuration.camera_change) < p_camera->get_bigger_side())
  {
    this->previous_camera_scale = p_camera->get_bigger_side();
    T minimum_visible_line_size = T(this->configuration.minimum_shape_size) * p_camera->get_bigger_side();

    for (auto &&stamp : this->all_stamps)
    {
      if (stamp != nullptr)
      {
        Shapes<T> shapes = stamp->update_on_zoom(
          p_camera, configuration.max_number_of_elements_in_memory,
          minimum_visible_line_size, 1000, last_depth_exponent_numerator
        );
        for (auto &&shape : shapes.get_shapes())
        {
          new_shapes.add_shape(shape);
        }
      }
    }
  }
}

template<typename T>
void StampPaintDataStructure<T>::process_confirm() {
  int current_stamp_number = 0;
  if (this->current_stamp != nullptr)
  {
    if (this->all_stamps[current_stamp_number] == nullptr)
    {
      this->all_stamps[current_stamp_number] = current_stamp;
      Shapes<T> shapes = this->all_stamps[current_stamp_number]->get_lines_recursive();
      for (auto &&shape : shapes.get_shapes())
      {
        new_shapes.add_shape(shape);
      }
    } else {
      Shapes<T> shapes = this->current_stamp->get_lines_recursive();
      for (auto &&shape : shapes.get_shapes())
      {
        new_shapes.add_shape(shape);
      }
      Shapes<T> child_reflected_shapes = this->all_stamps[current_stamp_number]->add_child_stamp(this->current_stamp);
      for (auto &&shape : child_reflected_shapes.get_shapes())
      {
        new_shapes.add_shape(shape);
      }
      T minimum_visible_line_size = T(this->configuration.minimum_shape_size) * p_camera->get_bigger_side();
      shapes = this->all_stamps[current_stamp_number]->update_on_zoom(
        p_camera, configuration.max_number_of_elements_in_memory,
        minimum_visible_line_size, 1000, last_depth_exponent_numerator
      );
      for (auto &&shape : shapes.get_shapes())
      {
        new_shapes.add_shape(shape);
      }
    }
    figure_was_drawn.push_back(false);
    current_stamp = nullptr;
    update_temporary_shapes();
  }
}
