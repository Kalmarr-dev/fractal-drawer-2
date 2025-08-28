#pragma once

#include "IViewport.fwrd.h"

class IWindowReconstructionObserver
{
public:
  virtual ~IWindowReconstructionObserver() = default;
  virtual void process_window_reconstruction(IViewport* p_viewport) = 0;
};