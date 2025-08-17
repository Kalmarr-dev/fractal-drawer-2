#pragma once

class IRenderer
{
public:
  virtual ~IRenderer();
  virtual void render_to_screen() = 0;
};