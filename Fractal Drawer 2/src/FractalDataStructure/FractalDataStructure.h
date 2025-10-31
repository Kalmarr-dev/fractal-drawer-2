#pragma once

#include <list>
#include <ShapeDataStructure/IShapeDataStructure.h>
#include <Camera/ICamera.h>
#include <Configuration/Configuration.h>
#include "Fractal/Fractal.h"
#include "Fractal/FractalPart.h"
#include "Fractal/FractalStub.h"
#include <Input/IPointerMoveObserver.h>
#include <Input/IConfirmObserver.h>
#include <Input/IZoomObserver.h>

template<typename T>
class FractalDataStructure : public IShapeDataStructure<T>, public IConfirmObserver, public IPointerMoveObserver<T>
{
private:
  FractalStub<T> fractal_stub;
  std::list<Fractal<T>*> all_fractals;
  std::list<Shapes<T>> all_fractal_shapes;
  Shapes<T> new_shapes;
  Shapes<T> temporary_shapes;
  ICamera<T>* p_camera;
  Configuration configuration;
  double minimum_visible_screen_size;
  T camera_scale_change_to_recalculate_fractals;
  T previous_camera_scale;

  void draw_silly_line(Position<T> pointer);
  void update_temporary_shapes(const Shapes<T>);
public:
  FractalDataStructure(ICamera<T>* p_camera, Configuration configuration);
  ~FractalDataStructure() = default;

  void clear_shapes() override;
  Shapes<T> clear_last_shapes() override;
  Shapes<T> get_new_shapes() override;
  Shapes<T> get_temporary_shapes() override;
  void process_pointer_move(Position<T> pointer) override;
  void process_primary_click(Position<T> pointer) override;
  void process_secondary_click(Position<T> pointer) override;
  void process_confirm() override;
  void process_zoom(Position<T> pointer, T scale_value) override;
};
