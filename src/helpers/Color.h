#pragma once

template<typename T>
struct Color
{
  T r;
  T g;
  T b;
  T a;

  Color() {
    Color::r = 0;
    Color::g = 0;
    Color::b = 0;
    Color::a = 1;
  }
  
  Color(T r, T g, T b) {
    Color::r = r;
    Color::g = g;
    Color::b = b;
    Color::a = 1;
  }

  Color(T r, T g, T b, T a) {
    Color::r = r;
    Color::g = g;
    Color::b = b;
    Color::a = a;
  }
};
