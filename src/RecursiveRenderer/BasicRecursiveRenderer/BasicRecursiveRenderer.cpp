#include "BasicRecursiveRenderer.h"

template<typename T>
BasicRecursiveRenderer<T>::BasicRecursiveRenderer
(
  IDataStructure2D<T>* p_data_structure_2d, ICamera<T>* p_camera,
  IShapeDataStructure<T>* p_shape_data_structure, const Configuration& configuration
) : p_data_structure_2d(p_data_structure_2d), p_camera(p_camera), 
  p_shape_data_structure(p_shape_data_structure), configuration(configuration) {}


template<typename T>
void BasicRecursiveRenderer<T>::update_data_structure_2d_with_new_shapes() {
  p_data_structure_2d->add_shapes(p_shape_data_structure->get_new_shapes());
}

template<typename T>
Shapes<T> BasicRecursiveRenderer<T>::get_shapes_on_camera() const {
  auto corners = this->p_camera->get_camera_corners();
  Shapes<T> shapes = this->p_data_structure_2d->get_visible_shapes_in_area(corners.first, corners.second);
  shapes.sort_by_size();
  shapes = shapes.get_first_n(configuration.max_number_of_elements_rendered);
  for (auto &&i : p_shape_data_structure->get_temporary_shapes().get_shapes())
  {
    shapes.add_shape(i);
  }
  return shapes;
}

// HAS TO BE CALLED AFTER CAMERA ZOOM
template<typename T>
void BasicRecursiveRenderer<T>::zoom(Position<T> pointer, T scale_value) {
  p_shape_data_structure->update_shapes_on_zoom();
  update_data_structure_2d_with_new_shapes();
}

template<typename T>
void BasicRecursiveRenderer<T>::zoom_reset() {
  this->p_camera->reset_camera_corners();
}

template<typename T>
void BasicRecursiveRenderer<T>::clear_shapes() {
  this->p_data_structure_2d->clear_shapes();
  this->p_shape_data_structure->clear_shapes();
}

template<typename T>
void BasicRecursiveRenderer<T>::process_primary_click(Position<T> pointer) {
  update_data_structure_2d_with_new_shapes();
}

template<typename T>
void BasicRecursiveRenderer<T>::process_secondary_click(Position<T> pointer) {
  update_data_structure_2d_with_new_shapes();
}
