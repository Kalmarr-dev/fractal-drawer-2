#include "BasicRecursiveRenderer.h"

template<typename T>
BasicRecursiveRenderer<T>::BasicRecursiveRenderer(IDataStructure2D<T>* p_data_structure, ICamera<T>* p_camera, const Configuration& configuration) {
  this->p_data_structure = p_data_structure;
  this->p_camera = p_camera;
  this->configuration = configuration;
}

// TODO sort them by size and cut away using configuration.max_number_of_elements_rendered
template<typename T>
Shapes<T> BasicRecursiveRenderer<T>::get_shapes_on_camera() const {
  auto corners = this->p_camera->get_camera_corners();
  return this->p_date_structure->get_visible_shapes_in_area(corners.first, corners.second);
}

template<typename T>
void BasicRecursiveRenderer<T>::zoom_in(Position<T> pointer, T scale_value) {
  this->p_camera->zoom_in(pointer, scale_value);
}
template<typename T>
void BasicRecursiveRenderer<T>::zoom_out(Position<T> pointer, T scale_value) {
  this->p_camera->zoom_out(pointer, T(1) / scale_value);
}
template<typename T>
void BasicRecursiveRenderer<T>::zoom_reset() {
  this->p_camera->reset_camera_corners();
}
template<typename T>
void BasicRecursiveRenderer<T>::clear_shapes() {
  this->p_data_structure->clear_shapes();
}