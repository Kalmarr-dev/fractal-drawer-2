#pragma once

class IViewportObserver
{
public:
  virtual ~IViewportObserver() = default;
  virtual void set_fullscreen_should_be_toggled() = 0;
};