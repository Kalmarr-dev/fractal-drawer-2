#pragma once

#include "IShape.h"
#include <vector>
#include <utility>
#include "../LongDouble/ILongDouble.h"

template<typename T>
class Line : public IShape<T>
{
private:
  ShapeType type = ShapeType::LINE;

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

  inline ShapeType get_type() const {
    return this->type;
  }

  inline Line* scale(double scale_x, double scale_y) const {
    return new Line({a.x * scale_x, a.y * scale_y}, {b.x * scale_x, b.y * scale_y});
  }

  inline Line* add_one_to_negative_coordinates() const {
    return new Line(
      {
        a.x < 0 ? a.x + 1 : a.x,
        a.y < 0 ? a.y + 1 : a.y
      }, {
        b.x < 0 ? b.x + 1 : b.x,
        b.y < 0 ? b.y + 1 : b.y
      }
    );
  }
};

