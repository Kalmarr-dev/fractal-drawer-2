#pragma once

#include <list>
#include <vector>
#include "../../../LongDouble/ILongDouble.h"
#include "../../../Shapes/Line.h"
#include "../../../Shapes/Shapes.h"
#include "../../../Shapes/IShape.h"
#include "../../../helpers/Position.h"

enum LineType { NONE, ROOT_LINE, DIRECTION_LINE };

template<typename T>
class FractalStub
{
private:
  LineType current_line_type = NONE;
  Position<T> current_line_first_point;
  Line<T> root_line;
  std::list<Line<T>> direction_lines;
public:
  FractalStub();
  FractalStub(Line<T> root_line, std::list<Line<T>> direction_lines);
  ~FractalStub() = default;

  Line<T> get_root_line();
  std::list<Line<T>> get_direction_lines();

  void clear();
  void add_point_to_root_line(Position<T> new_point);
  void add_point_to_direction_line(Position<T> new_point);
  Shapes<T> get_recursed_lines(int recursion_level);
  Shapes<T> get_recursed_lines(int recursion_level, Position<T> new_point);
};
