#pragma once

#include <list>
#include <set>
#include "../../../Camera/ICamera.h"
#include "FractalPart.h"

template<typename T>
class Fractal
{
private:
  FractalPart<T>* root_fractal_part;
  // std::list<FractalPart<T>*> all_fractal_parts;
  std::set<FractalPart<T>*> peripheral_fractal_parts;

public:
  Fractal(FractalPart<T>* root_fractal_part);
  ~Fractal() = default;

  std::vector<FractalPart<T>*> update_on_zoom(ICamera<T>* p_camera, int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH);
};
