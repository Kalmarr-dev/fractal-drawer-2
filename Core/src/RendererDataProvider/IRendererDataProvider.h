#pragma once

template<typename T>
class IRendererDataProvider
{
public:
  virtual ~IRendererDataProvider() = default;
  virtual Shapes<T> get_shapes_on_camera() const = 0;
};