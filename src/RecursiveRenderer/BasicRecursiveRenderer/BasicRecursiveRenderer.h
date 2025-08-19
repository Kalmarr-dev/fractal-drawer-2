#pragma once

#include "../../Configuration/Configuration.h"
#include "../../DataStructure2D/IDataStructure2D.h"
#include "../../Camera/ICamera.h"
#include "../IRecursiveRenderer.h"
#include "../IRecursiveRendererObserver.h"

template<typename T>
class BasicRecursiveRenderer : public IRecursiveRenderer<T>
{
private:
  IDataStructure2D<T>* p_data_structure;
  ICamera<T>* p_camera;
  Configuration configuration;

public:
  BasicRecursiveRenderer(IDataStructure2D<T>* data_structure, ICamera<T>* p_camera, const Configuration& configuration);
  ~BasicRecursiveRenderer() = default;

  Shapes<T> get_shapes_on_camera() const override;

  void zoom(Position<T> pointer, T scale_value) override;
  void zoom_reset() override;
  void clear_shapes() override;
  // void resize_camera(int screen_width, int screen_height) override;
};