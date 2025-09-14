#pragma once

#include "../helpers/Position.h"

class IClearObserver
{
public:
  virtual ~IClearObserver() = default;
  virtual void clear_shapes() = 0;
};