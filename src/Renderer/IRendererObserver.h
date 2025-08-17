#pragma once

class IRendererObserver
{
public:
  virtual ~IRendererObserver();
  virtual void toggle_fullscreen() = 0;
};