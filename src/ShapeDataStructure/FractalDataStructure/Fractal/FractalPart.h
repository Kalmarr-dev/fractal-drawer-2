#pragma once

#include "../../../LongDouble/ILongDouble.h"
#include "../../../Shapes/Line.h"
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

public:
  FractalPart(FractalStub<T>* p_fractal_stub, int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH);
  ~FractalPart() = default;

  Shapes<T> update_on_zoom();
  std::vector<Line<T>> get_lines();
};
