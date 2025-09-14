#pragma once

#include "../LongDouble/ILongDouble.h"
#include "IShape.fwd.h"
#include "ShapeType.h"
#include "Rectangle.h"
#include <utility>
#include <vector>

template<typename T>
class IShape
{
public:
  virtual ~IShape() = default;
  virtual bool collides_with(const Rectangle<T>& rectangle) const = 0;
  virtual inline T get_linear_size_squared() const = 0;
  virtual inline void set_depth(T) = 0;
  virtual inline T get_depth() const = 0;
  virtual inline const std::vector<Position<T>>& get_points() & = 0;
  virtual inline std::vector<Position<T>> get_points() const&& = 0;
  virtual inline std::vector<unsigned int> get_indexes() const = 0;
  virtual inline ShapeType get_type() const = 0;
  virtual inline IShape* scale(double scale_x, double scale_y) const = 0;
  virtual inline IShape* add_one_to_negative_coordinates() const = 0;
};
