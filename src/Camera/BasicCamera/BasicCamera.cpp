#include "BasicCamera.h"

template<typename T>
BasicCamera<T>::BasicCamera(std::pair<Position<T>, Position<T>> camera_corners_default) {
  this->camera_corners_default = camera_corners_default;
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
void BasicCamera<T>::zoom(Position<T> pointer, T scale_value) {
  T width = this->camera_corners.second.x - this->camera_corners.first.x;
  T height = this->camera_corners.second.y - this->camera_corners.first.y;
  T dX = pointer.x * width / 2;
  T dY = pointer.y * height / 2;
  T x = this->camera_corners.first.x;
  T y = this->camera_corners.first.y;
  x = x + width / 2;
  y = y + height / 2;
  x = x + dX - dX / scale_value;
  y = y + dY - dY / scale_value;
  width = width / scale_value;
  height = height / scale_value;
  x = x - width / 2;
  y = y - height / 2;
  this->camera_corners.first.x = x;
  this->camera_corners.first.y = y;
  this->camera_corners.second.x = x + width;
  this->camera_corners.second.y = y + height;
}

template<typename T>
void BasicCamera<T>::reset_camera_corners() {
  this->camera_corners = this->camera_corners_default;
}
