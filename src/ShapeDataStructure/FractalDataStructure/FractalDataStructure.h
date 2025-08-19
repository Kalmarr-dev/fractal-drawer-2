#pragma once

#include <list>
#include "../IShapeDataStructure.h"
#include "../../Camera/ICamera.h"
#include "Fractal/Fractal.h"

template<typename T>
class FractalDataStructure : public IShapeDataStructure<T>
{
private:
  Shapes<T> currently_drawn_fractal;
  // std::list<Fractal<T>> all_fractals;
  Shapes<T> all_shapes;
  Shapes<T> new_shapes;
  ICamera<T>* p_camera;

  void draw_silly_line(Position<T> pointer);
public:
  FractalDataStructure(ICamera<T>* p_camera);
  ~FractalDataStructure() = default;

  void clear_shapes() override;
  void update_shapes_on_zoom() override;
  Shapes<T> get_new_shapes() override;
  void process_primary_click(Position<T> pointer) override;
  void process_secondary_click(Position<T> pointer) override;
};
