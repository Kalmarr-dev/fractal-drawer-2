#pragma once

#include "../helpers/Position.h"
#include "../Input/IZoomObserver.h"
#include "../Input/IWindowResizeObserver.h"
#include "../Input/IZoomResetObserver.h"

template<typename T>
class ICamera : public IZoomObserver<T>, public IWindowResizeObserver, public IZoomResetObserver
{
public:
  virtual ~ICamera() = default;
  virtual std::pair<Position<T>, Position<T>> get_camera_corners() = 0;
  virtual void set_camera_corners(Position<T> a, Position<T> b) = 0;
  virtual void reset_camera_corners() = 0;
  virtual void resize(int window_width, int window_height) = 0;
  virtual T get_bigger_side() = 0;
};