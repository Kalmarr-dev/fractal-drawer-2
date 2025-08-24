#pragma once

#include "IOnScreenButton.h"

class OnScreenButton : public IOnScreenButton
{
protected:
  // negative coordinates mean aligning with the opposite side
  Position<double> offset;
  double width;
  double height;
  Shapes<double> shapes;
  Color<double> color;
  bool clicked = false;

  double transform_coordinate(double coordinate, double scale);
public:
  ~OnScreenButton() = default;
  // std::pair<Position<double>, Position<double>> get_clickable_area_corners() override;
  Shapes<double> get_shapes(double window_width, double window_height) override;
  Color<double> get_color() override;
  bool pointer_is_inside(Position<double> pointer, double window_width, double window_height) override;
  void set_clicked() override;
  void set_not_clicked() override;
  bool is_clicked() override;
};