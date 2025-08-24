#pragma once

#include <utility>
#include "../helpers/Position.h"
#include "../Shapes/Shapes.h"
#include "../helpers/Color.h"

class IOnScreenButton
{
public:
  virtual ~IOnScreenButton() = default;
  // negative coordinates mean aligning with the opposite side
  // virtual std::pair<Position<double>, Position<double>> get_clickable_area_corners() = 0;
  virtual Shapes<double> get_shapes(double window_width, double window_height) = 0;
  virtual Color<double> get_color() = 0;
  virtual bool pointer_is_inside(Position<double> pointer, double window_width, double window_height) = 0;
  virtual void set_clicked() = 0;
  virtual void set_not_clicked() = 0;
  virtual bool is_clicked() = 0;
};
