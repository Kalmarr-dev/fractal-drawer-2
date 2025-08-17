#pragma once

#include "../LongDouble/ILongDouble.h"
#include "IShape.h"

template<typename T = double>
class Line : public IShape<T>
{
public:
  ~Line();
};

