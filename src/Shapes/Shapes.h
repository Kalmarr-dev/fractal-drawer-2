#pragma once

#include <vector>
#include <algorithm>
#include "../LongDouble/ILongDouble.h"
#include "IShape.h"

template<typename T>
class Shapes
{
private:
  std::vector<IShape<T>*> shapes;

public:
  Shapes() {}
  Shapes(const std::vector<IShape<T>*>& shapes) : shapes(shapes) {}

  ~Shapes() = default;

  void add_shape(IShape<T>* shape) {
    shapes.push_back(shape);
  }

  std::vector<IShape<T>*> get_shapes() {
    return shapes;
  }

  void clear() {
    shapes.clear();
  }

  void sort_by_size() {
    std::sort(
      shapes.begin(),
      shapes.end(),
      [](IShape<T>* lhs, IShape<T>* rhs) {return lhs->get_linear_size_squared() < rhs->get_linear_size_squared();}
    );
  }

  Shapes<T> get_first_n(int n) {
    auto end = shapes.begin();
    std::advance(end, std::min(n, (int)shapes.size()));
    return Shapes<T>(std::vector<IShape<T>*>(shapes.begin(), end));
  }
};

