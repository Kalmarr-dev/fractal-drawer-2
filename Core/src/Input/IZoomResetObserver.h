#pragma once

class IZoomResetObserver
{
public:
  virtual ~IZoomResetObserver() = default;
  virtual void process_zoom_reset() = 0;
};