#pragma once

#include <unordered_set>
#include "../IDataStructure2D.h"
#include <Shapes/Shapes.h>

template<typename T>
class BasicDataStructure2D : public IDataStructure2D<T>
{
private:
  std::unordered_set<IShape<T>*> shapes_set;
public:
  ~BasicDataStructure2D() = default;

  void add_shapes(Shapes<T> shapes) override;
  // TODO return by reference here
  Shapes<T> get_visible_shapes_in_area(Position<T> lower, Position<T> higher) override;
  void clear_shapes() override;
  void clear_shapes(const Shapes<T>& shapes) override;
};
