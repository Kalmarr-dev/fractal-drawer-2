#pragma once

#include "BasicSquareOnScreenButton.h"
#include "../../Shapes/Line.h"
#include "../../Shapes/Rectangle.h"

IOnScreenButton* get_predefined_basic_on_screen_button_zoom_in() {
  Color<double> button_color(0.75, 0.75, 0.90);

  Position<double> button_offset(-100, 10);
  double width = 90;
  Shapes<double> button_shapes;
  button_shapes.add_shape(new Line<double>(Position<double>(-10 - 10, 55), Position<double>(-100 + 10, 55)));
  button_shapes.add_shape(new Line<double>(Position<double>(-55, 100 - 10), Position<double>(-55, 10 + 10)));
  // button_shapes.add_shape(new Rectangle<double>(button_offset, {button_offset.x + width, button_offset.y + width}));
  button_shapes.add_shape(new Line<double>(Position<double>(button_offset.x, button_offset.y), Position<double>(button_offset.x, button_offset.y + width)));
  button_shapes.add_shape(new Line<double>(Position<double>(button_offset.x, button_offset.y + width), Position<double>(button_offset.x + width, button_offset.y + width)));
  button_shapes.add_shape(new Line<double>(Position<double>(button_offset.x + width, button_offset.y + width), Position<double>(button_offset.x + width, button_offset.y)));
  button_shapes.add_shape(new Line<double>(Position<double>(button_offset.x + width, button_offset.y), Position<double>(button_offset.x, button_offset.y)));
  BasicSquareOnScreenButton* button_zoom_in = new BasicSquareOnScreenButton
  (
    button_offset,
    width,
    button_shapes,
    button_color
  );

  return button_zoom_in;
}

IOnScreenButton* get_predefined_basic_on_screen_button_zoom_out() {
  Color<double> button_color(0.75, 0.75, 0.90);

  Position<double> button_offset(-200, 10);
  double width = 90;
  Shapes<double> button_shapes;
  button_shapes.add_shape(new Line<double>(Position<double>(-110 - 10, 55), Position<double>(-200 + 10, 55)));
  // button_shapes.add_shape(new Rectangle<double>(button_offset, {button_offset.x + width, button_offset.y + width}));
  button_shapes.add_shape(new Line<double>(Position<double>(button_offset.x, button_offset.y), Position<double>(button_offset.x, button_offset.y + width)));
  button_shapes.add_shape(new Line<double>(Position<double>(button_offset.x, button_offset.y + width), Position<double>(button_offset.x + width, button_offset.y + width)));
  button_shapes.add_shape(new Line<double>(Position<double>(button_offset.x + width, button_offset.y + width), Position<double>(button_offset.x + width, button_offset.y)));
  button_shapes.add_shape(new Line<double>(Position<double>(button_offset.x + width, button_offset.y), Position<double>(button_offset.x, button_offset.y)));
  BasicSquareOnScreenButton* button_zoom_in = new BasicSquareOnScreenButton
  (
    button_offset,
    width,
    button_shapes,
    button_color
  );

  return button_zoom_in;
}