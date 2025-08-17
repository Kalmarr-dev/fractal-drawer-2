#pragma once

#include "../helpers/Position.h"

template<typename T>
class ICamera
{
public:
  virtual ~ICamera();
  virtual std::pair<Position<T>, Position<T>> get_camera_corners() = 0;
  virtual void set_camera_corners(Position<T> a, Position<T> b) = 0;
  virtual void zoom(Position<T> pointer, T scale_value) = 0;
  virtual void reset_camera_corners() = 0;
};