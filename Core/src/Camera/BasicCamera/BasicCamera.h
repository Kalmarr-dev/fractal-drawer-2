#pragma once

#include <utility>
#include "../../helpers/Position.h"
#include "../ICamera.h"

template<typename T>
class BasicCamera : public ICamera<T>
{
private:
  std::pair<Position<T>, Position<T>> camera_corners;
  std::pair<Position<T>, Position<T>> camera_corners_default;
  int last_window_width = 0;
  int last_window_height = 0;
public:
  BasicCamera(std::pair<Position<T>, Position<T>> camera_corners_default);
  ~BasicCamera() = default;
  std::pair<Position<T>, Position<T>> get_camera_corners() override;
  void set_camera_corners(Position<T> a, Position<T> b) override;
  void process_zoom(Position<T> pointer, T scale_value) override;
  void reset_camera_corners() override;
  void resize(int window_width, int window_height) override;
  void process_window_resize(int width, int height) override;
  void process_zoom_reset() override;
  T get_bigger_side() override;
};