#pragma once

#include "../helpers/Position.h"

template<typename T>
class IZoomObserver
{
public:
  virtual ~IZoomObserver() = default;
  virtual void zoom(Position<T> pointer, T scale_value) = 0;
};