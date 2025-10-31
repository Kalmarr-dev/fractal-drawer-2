#include "OnScreenButton.h"

// std::pair<Position<double>, Position<double>> OnScreenButton::get_clickable_area_corners() {
//  return std::make_pair(corner_1, corner_2);
// }

double OnScreenButton::transform_coordinate(double coordinate, double scale) {
  coordinate *= scale;
  if (coordinate < 0) coordinate += 1;
  return coordinate;
}

Shapes<double> OnScreenButton::get_shapes(double window_width, double window_height) {
  return shapes;
}

Color<double> OnScreenButton::get_color() {
  return color;
}

bool OnScreenButton::pointer_is_inside(Position<double> pointer, double window_width, double window_height) {
  double min_x = std::min(offset.x, offset.x + width);
  min_x = transform_coordinate(min_x, 1.0 / window_width);
  double max_x = std::max(offset.x, offset.x + width);
  max_x = transform_coordinate(max_x, 1.0 / window_width);
  double min_y = std::min(offset.y, offset.y + height);
  min_y = transform_coordinate(min_y, 1.0 / window_height);
  double max_y = std::max(offset.y, offset.y + height);
  max_y = transform_coordinate(max_y, 1.0 / window_height);

  // std::cout << min_x << " " << max_x << " " << min_y << " " << max_y << " " << window_width << " " << window_height << '\n';
  
  if (pointer.x < min_x || max_x < pointer.x || pointer.y < min_y || max_y < pointer.y)
  {
    return false;
  } else {
    return true;
  }
}

void OnScreenButton::set_clicked() {
  clicked = true;
}

void OnScreenButton::set_not_clicked() {
  clicked = false;
}

bool OnScreenButton::is_clicked() {
  return clicked;
}
