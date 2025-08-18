#pragma once

class IRenderer
{
public:
  virtual ~IRenderer() = default;
  virtual void render_to_screen() = 0;
  virtual void clear_screen() = 0;
};