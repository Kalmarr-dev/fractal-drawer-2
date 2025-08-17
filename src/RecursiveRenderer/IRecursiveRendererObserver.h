#pragma once

template<typename T>
class IRecursiveRendererObserver
{
public:
  virtual ~IRecursiveRendererObserver() = default;
  virtual void zoom_in(Position<T> pointer, T scale_value) = 0;
  virtual void zoom_out(Position<T> pointer, T scale_value) = 0;
  virtual void zoom_reset() = 0;
  virtual void clear_shapes() = 0;
};