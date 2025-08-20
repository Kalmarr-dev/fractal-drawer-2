#include "FractalStub.h"

#include "FractalPart.h"

template<typename T>
FractalStub<T>::FractalStub() : root_line(Line<T>({-1,1},{1,1})) {}

template<typename T>
FractalStub<T>::FractalStub(Line<T> root_line, std::list<Line<T>> direction_lines)
  : root_line(root_line), direction_lines(direction_lines) {}

template<typename T>
void FractalStub<T>::clear() {
  root_line = Line<T>({-1,1},{1,1});
  direction_lines.clear();
}

template<typename T>
Line<T> FractalStub<T>::get_root_line() {
  return root_line;
}

template<typename T>
std::list<Line<T>> FractalStub<T>::get_direction_lines() {
  return direction_lines;
}

template<typename T>
void FractalStub<T>::add_point_to_root_line(Position<T> new_point) {
  if (current_line_type == LineType::ROOT_LINE)
  {
    root_line = Line<T>(current_line_first_point, new_point);
    current_line_type = LineType::NONE;
  } else {
    current_line_type = LineType::ROOT_LINE;
    current_line_first_point = new_point;
  }
}

template<typename T>
void FractalStub<T>::add_point_to_direction_line(Position<T> new_point) {
  if (current_line_type == LineType::DIRECTION_LINE)
  {
    direction_lines.push_back(Line<T>(current_line_first_point, new_point));
    current_line_type = LineType::NONE;
  } else {
    current_line_type = LineType::DIRECTION_LINE;
    current_line_first_point = new_point;
  }
}

template<typename T>
Shapes<T> FractalStub<T>::get_recursed_lines(int recursion_level) {
  std::vector<IShape<T>*> shapes;
  shapes.push_back(&root_line);
  for (auto &&i : direction_lines)
  {
    shapes.push_back(&i);
  }
  
  FractalPart<T> fractal_part(this, 100000, T(0.0), recursion_level);

  for (auto &&i : fractal_part.get_lines())
  {
    Line<T>* p_line = new Line<T>(i.a, i.b);
    shapes.push_back(p_line);
  }
  
  
  return Shapes<T>(shapes);
}
