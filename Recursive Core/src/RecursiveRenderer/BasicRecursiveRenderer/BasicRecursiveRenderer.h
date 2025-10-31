#pragma once

#include <Configuration/Configuration.h>
#include <DataStructure2D/IDataStructure2D.h>
#include <Camera/ICamera.h>
#include <ShapeDataStructure/IShapeDataStructure.h>
#include "../IRecursiveRenderer.h"

template<typename T>
class BasicRecursiveRenderer : public IRecursiveRenderer<T>
{
private:
  IDataStructure2D<T>* p_data_structure_2d;
  ICamera<T>* p_camera;
  IShapeDataStructure<T>* p_shape_data_structure;
  Configuration configuration;

  void update_data_structure_2d_with_new_shapes();
public:
  BasicRecursiveRenderer(IDataStructure2D<T>* data_structure_2d, ICamera<T>* p_camera, IShapeDataStructure<T>* p_shape_data_structure, const Configuration& configuration);
  ~BasicRecursiveRenderer() = default;

  Shapes<T> get_shapes_on_camera() const override;

  void process_zoom(Position<T> pointer, T scale_value) override;
  void process_zoom_reset() override;
  void clear_shapes() override;
  // void process_primary_click(Position<T> pointer) override;
  // void process_secondary_click(Position<T> pointer) override;
  void process_pointer_up() override;
  void process_confirm() override;
};