#pragma once

#include "../OnScreenButton.h"

class BasicSquareOnScreenButton : public OnScreenButton
{
public:
  BasicSquareOnScreenButton(Position<double> offset, double width, Shapes<double> shapes, Color<double> color);
  ~BasicSquareOnScreenButton() = default;
  Shapes<double> get_shapes(double window_width, double window_height) override;
};