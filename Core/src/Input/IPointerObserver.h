#pragma once

#include "../helpers/Position.h"

template<typename T>
class IPointerObserver
{
public:
  virtual ~IPointerObserver() = default;
  virtual void process_primary_click(Position<T> pointer) = 0;
  virtual void process_secondary_click(Position<T> pointer) = 0;
};