#pragma once

class IRecursiveRendererObserver
{
public:
  virtual ~IRecursiveRendererObserver();
  virtual void zoom_in() = 0;
  virtual void zoom_out() = 0;
  virtual void zoom_reset() = 0;
  virtual void clear_shapes() = 0;
};