#include "BasicCamera.h"

#include <algorithm>
#include <iostream>

template<typename T>
BasicCamera<T>::BasicCamera(std::pair<Position<T>, Position<T>> camera_corners_default) {
  this->camera_corners = this->camera_corners_default = camera_corners_default;
}

template<typename T>
std::pair<Position<T>, Position<T>> BasicCamera<T>::get_camera_corners() {
  return this->camera_corners;
}

template<typename T>
void BasicCamera<T>::set_camera_corners(Position<T> a, Position<T> b) {
  if (b.x < a.x)
  {
    std::swap(a.x, b.x);
  }
  if (b.y < a.y)
  {
    std::swap(a.y, b.y);
  }
  this->camera_corners = std::make_pair(a, b);
}

template<typename T>
void BasicCamera<T>::process_zoom(Position<T> pointer, T scale_value) {
  T width = this->camera_corners.second.x - this->camera_corners.first.x;
  T height = this->camera_corners.second.y - this->camera_corners.first.y;
  T dX = (pointer.x - T(0.5)) * width;
  T dY = (pointer.y - T(0.5)) * height;
  T x = this->camera_corners.first.x;
  T y = this->camera_corners.first.y;
  x = x + width / T(2.0);
  y = y + height / T(2.0);
  x = x + dX - dX / scale_value;
  y = y + dY - dY / scale_value;
  width = width / scale_value;
  height = height / scale_value;
  x = x - width / T(2.0);
  y = y - height / T(2.0);
  this->camera_corners.first.x = x;
  this->camera_corners.first.y = y;
  this->camera_corners.second.x = x + width;
  this->camera_corners.second.y = y + height;
}

template<typename T>
void BasicCamera<T>::reset_camera_corners() {
  this->camera_corners = this->camera_corners_default;
}

template<typename T>
void BasicCamera<T>::resize(int window_width, int window_height) {
  int smaller = std::min(window_width, window_height);
  T horizontalScaleF = (T)window_width / (T)smaller;
  T verticalScaleF = (T)window_height / (T)smaller;
  T width = this->camera_corners.second.x - this->camera_corners.first.x;
  T height = this->camera_corners.second.y - this->camera_corners.first.y;
  T x = this->camera_corners.first.x;
  T y = this->camera_corners.first.y;
  T smallerSide = std::min(width, height);
  x = x + width * T(.5) - smallerSide * horizontalScaleF * T(.5);
  y = y + height * T(.5) - smallerSide * verticalScaleF * T(.5);
  width = smallerSide * horizontalScaleF;
  height = smallerSide * verticalScaleF;
  this->camera_corners.first.x = x;
  this->camera_corners.first.y = y;
  this->camera_corners.second.x = x + width;
  this->camera_corners.second.y = y + height;
}

template<typename T>
void BasicCamera<T>::process_window_resize(int window_width, int window_height) {
  resize(window_width, window_height);
  this->last_window_width = window_width;
  this->last_window_height = window_height;
}

template<typename T>
void BasicCamera<T>::process_zoom_reset() {
  reset_camera_corners();
  resize(last_window_width, last_window_height);
}

template<typename T>
T BasicCamera<T>::get_bigger_side() {
  return std::max(camera_corners.second.x - camera_corners.first.x, camera_corners.second.y - camera_corners.first.y);
}

