#pragma once

#include "../helpers/Position.h"

template<typename T>
class IPointerMoveObserver
{
public:
  virtual ~IPointerMoveObserver() = default;
  virtual void process_primary_click(Position<T> pointer) = 0;
  virtual void process_pointer_move(Position<T> pointer) = 0;
};