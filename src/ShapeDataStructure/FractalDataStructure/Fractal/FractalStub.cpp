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
  shapes.push_back(new Line<T>(Position<T>(root_line.a.x, root_line.a.y), Position<T>(root_line.b.x, root_line.b.y)));
  for (auto &&i : direction_lines)
  {
    shapes.push_back(new Line<T>(Position<T>(i.a.x, i.a.y), Position<T>(i.b.x, i.b.y)));
  }
  
  FractalPart<T> fractal_part(this, 100000, T(0.0), recursion_level);

  for (auto &&i : fractal_part.get_lines())
  {
    Line<T>* p_line = new Line<T>(i.a, i.b);
    shapes.push_back(p_line);
  }
  
  
  return Shapes<T>(shapes);
}

template<typename T>
Shapes<T> FractalStub<T>::get_recursed_lines(int recursion_level, Position<T> new_point) {
  std::vector<IShape<T>*> shapes;
  if (current_line_type != LineType::NONE) {
    shapes.push_back(new Line<T>(Position<T>(current_line_first_point.x, current_line_first_point.y), Position<T>(new_point.x, new_point.y)));
  }
  Line<T> temporary_root_line;
  if (current_line_type != LineType::ROOT_LINE)
  {
    temporary_root_line = Line<T>(Position<T>(root_line.a.x, root_line.a.y), Position<T>(root_line.b.x, root_line.b.y));
    // shapes.push_back(new Line<T>(Position<T>(root_line.a.x, root_line.a.y), Position<T>(root_line.b.x, root_line.b.y)));
  } else {
    temporary_root_line = Line<T>(Position<T>(current_line_first_point.x, current_line_first_point.y), Position<T>(new_point.x, new_point.y));
  }
  
  std::list<Line<T>> temporary_direction_lines;
  for (auto &&i : direction_lines)
  {
    temporary_direction_lines.push_back(Line<T>(Position<T>(i.a.x, i.a.y), Position<T>(i.b.x, i.b.y)));
    // shapes.push_back(new Line<T>(Position<T>(i.a.x, i.a.y), Position<T>(i.b.x, i.b.y)));
  }
  if (current_line_type == LineType::DIRECTION_LINE)
  {
    temporary_direction_lines.push_back(Line<T>(Position<T>(current_line_first_point.x, current_line_first_point.y), Position<T>(new_point.x, new_point.y)));
  }
  
  FractalStub<T>* temporary_fractal_stub = new FractalStub<T>(temporary_root_line, temporary_direction_lines);
  FractalPart<T> fractal_part(temporary_fractal_stub, 100000, T(0.0), recursion_level);
  delete temporary_fractal_stub;

  for (auto &&i : fractal_part.get_lines())
  {
    Line<T>* p_line = new Line<T>(i.a, i.b);
    shapes.push_back(p_line);
  }
  
  
  return Shapes<T>(shapes);
}
