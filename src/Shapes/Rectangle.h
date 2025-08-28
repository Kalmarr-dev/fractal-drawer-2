#pragma once

#include <utility>
#include <vector>
#include "../LongDouble/ILongDouble.h"
#include "../helpers/Position.h"
#include "IShape.h"
#include "ShapeType.h"

template<typename T>
class Rectangle : public IShape<T>
{
private:
  Position<T> lower;
  Position<T> higher;
  ShapeType type = ShapeType::RECTANGLE;
public:
  Rectangle(Position<T> a, Position<T> b) {
    if (b.x < a.x)
    {
      std::swap(a.x, b.x);
    }
    if (b.y < a.y)
    {
      std::swap(a.y, b.y);
    }
    this->lower = a;
    this->higher = b;
  }

  ~Rectangle() = default;

  bool collides_with(const Rectangle<T>& rectangle) const {
    throw "not implemented";
  }

  inline T get_linear_size_squared() const {
    return (higher.x - lower.x) * (higher.x - lower.x) + (higher.y - lower.y) * (higher.y - lower.y);
  }

  inline std::vector<Position<T>> get_points() const {
    std::vector<Position<T>> positions;
    positions.push_back(lower);
    positions.push_back({higher.x, lower.y});
    positions.push_back(higher);
    positions.push_back({lower.x, higher.y});
    return positions;
  }

  inline std::vector<unsigned int> get_indexes() const {
    std::vector<unsigned int> indexes;
    indexes.push_back(0);
    indexes.push_back(1);
    indexes.push_back(1);
    indexes.push_back(2);
    indexes.push_back(2);
    indexes.push_back(3);
    indexes.push_back(3);
    indexes.push_back(0);
    return indexes;
  }

  inline ShapeType get_type() const {
    return this->type;
  }

  inline Rectangle* scale(double scale_x, double scale_y) const {
    return new Rectangle({lower.x * T(scale_x), lower.y * T(scale_y)}, {higher.x * T(scale_x), higher.y * T(scale_y)});
  }

  inline Rectangle* add_one_to_negative_coordinates() const {
    return new Rectangle(
      {
        lower.x < T(0) ? lower.x + T(1) : lower.x,
        lower.y < T(0) ? lower.y + T(1) : lower.y
      }, {
        higher.x < T(0) ? higher.x + T(1) : higher.x,
        higher.y < T(0) ? higher.y + T(1) : higher.y
      }
    );
  }
};

