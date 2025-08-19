#pragma once

#include "../helpers/Position.h"

class IWindowResizeObserver
{
public:
  virtual ~IWindowResizeObserver() = default;
  virtual void process_window_resize(int width, int height) = 0;
};