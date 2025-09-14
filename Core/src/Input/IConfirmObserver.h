#pragma once

#include "../helpers/Position.h"

class IConfirmObserver
{
public:
  virtual ~IConfirmObserver() = default;
  virtual void process_confirm() = 0;
};