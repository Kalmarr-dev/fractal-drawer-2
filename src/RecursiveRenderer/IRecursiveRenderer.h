#pragma once

#include "../helpers/Position.h"
#include "../Shapes/Shapes.h"
#include "IRecursiveRendererObserver.h"
#include "../Input/IZoomObserver.h"

template<typename T>
class IRecursiveRenderer : public IRecursiveRendererObserver<T>, public IZoomObserver<T>
{
public:
  virtual ~IRecursiveRenderer() = default;
  virtual Shapes<T> get_shapes_on_camera() const = 0;
};