#pragma once

#include "../helpers/Position.h"
#include "../Input/IZoomObserver.h"
#include "../Input/IWindowResizeObserver.h"

template<typename T>
class ICamera : public IZoomObserver<T>, public IWindowResizeObserver
{
public:
  virtual ~ICamera() = default;
  virtual std::pair<Position<T>, Position<T>> get_camera_corners() = 0;
  virtual void set_camera_corners(Position<T> a, Position<T> b) = 0;
  virtual void zoom(Position<T> pointer, T scale_value) = 0;
  virtual void reset_camera_corners() = 0;
  virtual void resize(int window_width, int window_height) = 0;
};