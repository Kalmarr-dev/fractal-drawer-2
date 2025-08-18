#pragma once

#include "../Viewport/IViewport.fwrd.h"
#include "../Viewport/IViewport.h"

class IInputObserver
{
public:
  virtual ~IInputObserver() = default;
  virtual void subscribe_viewport_to_callbacks(IViewport* p_viewport) = 0;
};