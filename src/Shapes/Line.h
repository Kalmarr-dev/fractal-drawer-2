#pragma once

#include "../LongDouble/ILongDouble.h"
#include "IShape.h"

template<typename T>
class Line : public IShape<T>
{
public:
  ~Line() = default;
};

