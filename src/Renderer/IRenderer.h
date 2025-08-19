#pragma once

#include "../Input/IWindowResizeObserver.h"

class IRenderer : public IWindowResizeObserver
{
public:
  virtual ~IRenderer() = default;
  virtual void render_to_screen() = 0;
  virtual void clear_screen() = 0;
};