#pragma once

#include <vector>
#include <algorithm>
#include <execution>
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

  const std::vector<IShape<T>*> get_shapes() const {
    return shapes;
  }

  size_t size() {
    return shapes.size();
  }

  void clear() {
    shapes.clear();
  }

  void sort_by_size() {
    std::sort(
      shapes.rbegin(),
      shapes.rend(),
      [](IShape<T>* lhs, IShape<T>* rhs) {return lhs->get_linear_size_squared() < rhs->get_linear_size_squared();}
    );
  }

  Shapes<T> get_first_n(int n) {
    // auto end = shapes.begin();
    // std::advance(end, std::min(n, (int)shapes.size()));
    // return Shapes<T>(std::vector<IShape<T>*>(shapes.begin(), end));
    
    Shapes<T> new_shapes;
    new_shapes.shapes.resize(std::min(n, (int)shapes.size()));
    auto end = shapes.begin();
    std::advance(end, std::min(n, (int)shapes.size()));
    // std::copy(std::execution::par_unseq, this->shapes.begin(), end, new_shapes.get_shapes().begin());
    std::copy(std::execution::par_unseq, this->shapes.begin(), end, new_shapes.shapes.begin());
    return new_shapes;
  }
};

