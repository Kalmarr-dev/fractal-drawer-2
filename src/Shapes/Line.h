#pragma once

#include "IShape.h"
#include <vector>
#include <utility>
#include "../LongDouble/ILongDouble.h"

template<typename T>
class Line : public IShape<T>
{
private:
  Position<T> lower;
  Position<T> higher;
public:
  Line(Position<T> a, Position<T> b) {
    if (b.y < a.y)
    {
      std::swap(a.x, b.x);
      std::swap(a.y, b.y);
    }
    this->lower = a;
    this->higher = b;
  }

  ~Line() = default;

  bool collides_with(const Rectangle<T>& rectangle) {
    throw "not implemented";
  }

  inline T get_linear_size_squared() {
    return (higher.x - lower.x) * (higher.x - lower.x) + (higher.y - lower.y) * (higher.y - lower.y);
  }

  inline std::vector<Position<T>> get_points() {
    std::vector<Position<T>> positions;
    positions.push_back(lower);
    positions.push_back(higher);
    return positions;
  }

  inline std::vector<unsigned int> get_indexes() {
    std::vector<unsigned int> indexes;
    indexes.push_back(0);
    indexes.push_back(1);
    return indexes;
  }
};

