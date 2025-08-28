#pragma once

#include <set>
#include <utility>
#include "../../../helpers/Position.h"

template<typename T>
struct ScreenCoordinateSquares
{
public:
  std::set<std::pair<int, int>> square_coordinates;
  const Position<T> origin;
  const T square_size;
  ScreenCoordinateSquares() = delete;
  ScreenCoordinateSquares(Position<T> origin, T square_size)
    : origin(origin), square_size(square_size) {}
  ~ScreenCoordinateSquares() = default;
  void add_circle_of_squares(const Position<T> position, const T circle_radius) {
    T raduis_in_screen_coordinates = circle_radius / square_size;
    T shift_x = position.x - origin.x;
    T shift_y = position.y - origin.y;
    T* offset_zero = new T(0);
    for (int i = (int)(shift_x - raduis_in_screen_coordinates).get_double(offset_zero, 0); i < (shift_x + raduis_in_screen_coordinates).get_double(offset_zero, 0); i++)
    {
      for (int j = (int)(shift_y - raduis_in_screen_coordinates).get_double(offset_zero, 0); j < (shift_y + raduis_in_screen_coordinates).get_double(offset_zero, 0); j++)
      {
        if (i * i + j * j < (raduis_in_screen_coordinates * raduis_in_screen_coordinates).get_double(offset_zero, 0))
        {
          square_coordinates.insert(std::make_pair(i, j));
        }        
      }
    }
  }
};
