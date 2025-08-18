#pragma once

#include <utility>
#include "IViewport.fwrd.h"
#include "../Input/IInputObserver.h"

class IViewport
{
public:
  virtual ~IViewport() = default;
  virtual std::pair<int, int> get_size() = 0;

  virtual void subscribe_to_window_reconstruction(IInputObserver* observer) = 0;
  virtual void unsubscribe_from_window_reconstruction(IInputObserver* observer) = 0;
  virtual void notify_window_reconstruction() = 0;

  virtual bool window_should_close() = 0;
  virtual void toggle_fullscreen() = 0;
};
