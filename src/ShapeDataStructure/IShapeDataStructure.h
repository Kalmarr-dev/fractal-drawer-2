#pragma once

#include "../LongDouble/ILongDouble.h"
#include "../Shapes/Shapes.h"
#include "../Input/IPointerObserver.h"
#include "../Camera/ICamera.h"

template<typename T>
class IShapeDataStructure : public IPointerObserver<T>
{
public:
  virtual ~IShapeDataStructure() = default;

  virtual void clear_shapes() = 0;
  virtual void update_shapes_on_zoom() = 0;
  virtual Shapes<T> get_new_shapes() = 0;
  virtual Shapes<T> get_temporary_shapes() = 0;
};
