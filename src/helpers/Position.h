#pragma once

template<typename T>
struct Position
{
  T x;
  T y;

  Position() {
    Position::x = 0.0;
    Position::y = 0.0;
  }
  
  Position(T x, T y) {
    Position::x = x;
    Position::y = y;
  }
};
