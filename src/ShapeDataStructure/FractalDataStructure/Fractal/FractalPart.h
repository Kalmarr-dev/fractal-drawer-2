#pragma once

#include <set>
#include "../../../LongDouble/ILongDouble.h"
#include "../../../Shapes/Line.h"
#include "../../../Camera/ICamera.h"
#include "FractalStub.h"

template<typename T>
class FractalPart
{
private:
  Line<T> root_line;
  std::vector<Line<T>> direction_lines;
  std::vector<Line<T>> lines;
  std::vector<int> linesOnTheLayer;
  std::vector<Line<T>> lastReflectionLines;
  std::set<Line<T>*> used_last_reflection_lines;
  T linear_size = 0;
  T min_x = 0;
  T max_x = 0;
  T min_y = 0;
  T max_y = 0;

public:
  FractalPart(FractalStub<T>* p_fractal_stub, int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH);
  ~FractalPart() = default;

  std::vector<Line<T>> get_lines();
  T get_size();
  std::pair<Position<T>, Position<T>> get_corners();
  static bool intersects_with(std::pair<Position<T>, Position<T>> sorted_corners, std::pair<Position<T>, Position<T>> corners);
  Line<T> get_root_line();
  std::vector<Line<T>> get_direction_lines();
  std::vector<Line<T>> get_last_reflection_lines();
  void insert_used_last_reflection_line(Line<T>* p_line);
  bool find_used_last_reflection_line(Line<T>* p_line);
  int get_used_last_reflection_lines_size();
};
