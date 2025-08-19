#pragma once

#include "IShape.h"
#include <vector>
#include <utility>
#include "../LongDouble/ILongDouble.h"

template<typename T>
class Line : public IShape<T>
{
private:
public:
  Position<T> a;
  Position<T> b;

  Line() {
    this->a = Position<T>((0), T(0));
    this->b = Position<T>((0), T(0));
  }

  Line(Position<T> a, Position<T> b) {
    this->a = a;
    this->b = b;
  }

  ~Line() = default;

  bool collides_with(const Rectangle<T>& rectangle) const {
    throw "not implemented";
  }

  inline T get_linear_size_squared() const {
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
  }

  inline std::vector<Position<T>> get_points() const {
    std::vector<Position<T>> positions;
    positions.push_back(a);
    positions.push_back(b);
    return positions;
  }

  inline std::vector<unsigned int> get_indexes() const {
    std::vector<unsigned int> indexes;
    indexes.push_back(0);
    indexes.push_back(1);
    return indexes;
  }
};

