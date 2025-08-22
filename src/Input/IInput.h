#pragma once

class IInput
{
public:
  virtual ~IInput() = default;
  virtual void send_recurring_events() = 0;
};