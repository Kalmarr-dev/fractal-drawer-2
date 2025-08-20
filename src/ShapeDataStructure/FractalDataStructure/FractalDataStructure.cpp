#include "FractalDataStructure.h"

#include "../../Shapes/Line.h"

template<typename T>
FractalDataStructure<T>::FractalDataStructure(ICamera<T>* p_camera, Configuration configuration) 
  : p_camera(p_camera), configuration(configuration) {
  this->minimum_visible_screen_size = 0.00125;
  this->camera_scale_change_to_recalculate_fractals = 4;
  // this->camera_scale_change_to_recalculate_fractals = 2;
  this->previous_camera_scale = p_camera->get_bigger_side();
}

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
void FractalDataStructure<T>::update_temporary_shapes(const Shapes<T> shapes) {
  temporary_shapes.clear();
  for (auto &&i : shapes.get_shapes())
  {
    temporary_shapes.add_shape(i);
  }
}

template<typename T>
void FractalDataStructure<T>::clear_shapes() {
  fractal_stub.clear();
  all_fractals.clear();
  all_shapes.clear();
  new_shapes.clear();
}

template<typename T>
void FractalDataStructure<T>::process_zoom(Position<T> pointer, T scale_value) {
  if (p_camera->get_bigger_side() < this->previous_camera_scale / this->camera_scale_change_to_recalculate_fractals
  || this->previous_camera_scale * this->camera_scale_change_to_recalculate_fractals < p_camera->get_bigger_side())
  {
    this->previous_camera_scale = p_camera->get_bigger_side();
    
    T minimum_visible_line_size = T(this->minimum_visible_screen_size) * p_camera->get_bigger_side();
    for (auto &&fractal : all_fractals) // TODO heavy three loops
    {
      auto parts = fractal->update_on_zoom(this->p_camera, configuration.max_number_of_elements_in_memory, minimum_visible_line_size, 100);
      for (auto &&part : parts)
      {
        for (auto &&i : part->get_lines())
        {
          Line<T>* line = new Line<T>(i.a, i.b);
          all_shapes.add_shape(line);
          new_shapes.add_shape(line);
        }
      }
    }
  }
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

  const Shapes<T>& shapes = fractal_stub.get_recursed_lines(4);
  update_temporary_shapes(shapes);
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

  const Shapes<T>& shapes = fractal_stub.get_recursed_lines(4);
  update_temporary_shapes(shapes);
}

template<typename T>
void FractalDataStructure<T>::process_confirm() {
  T minimum_visible_line_size = T(this->minimum_visible_screen_size) * p_camera->get_bigger_side();
  FractalPart<T>* fractal_part = new FractalPart<T>(&this->fractal_stub, configuration.max_number_of_elements_in_memory, minimum_visible_line_size, 100);
  Fractal<T>* fractal = new Fractal<T>(fractal_part);
  all_fractals.push_back(fractal);

  this->fractal_stub = FractalStub<T>();
  const Shapes<T>& shapes = fractal_stub.get_recursed_lines(4);
  update_temporary_shapes(shapes);

  auto lines = fractal_part->get_lines();

  for (auto &&i : lines)
  {
    Line<T>* line = new Line<T>(i.a, i.b);
    all_shapes.add_shape(line);
    new_shapes.add_shape(line);
  }
}
