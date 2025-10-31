#pragma once

class IPointerUpObserver
{
public:
  virtual ~IPointerUpObserver() = default;
  virtual void process_pointer_up() = 0;
};