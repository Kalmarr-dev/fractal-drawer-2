#pragma once

#include <list>
#include "../IShapeDataStructure.h"
#include "../../Camera/ICamera.h"
#include "Fractal/FractalPart.h"
#include "Fractal/FractalStub.h"
#include "../../Input/IConfirmObserver.h"

template<typename T>
class FractalDataStructure : public IShapeDataStructure<T>, public IConfirmObserver
{
private:
  FractalStub<T> fractal_stub;
  std::list<FractalPart<T>> all_fractals;
  Shapes<T> all_shapes;
  Shapes<T> new_shapes;
  Shapes<T> temporary_shapes;
  ICamera<T>* p_camera;

  void draw_silly_line(Position<T> pointer);
  void update_temporary_shapes(Shapes<T>);
public:
  FractalDataStructure(ICamera<T>* p_camera);
  ~FractalDataStructure() = default;

  void clear_shapes() override;
  void update_shapes_on_zoom() override;
  Shapes<T> get_new_shapes() override;
  Shapes<T> get_temporary_shapes() override;
  void process_primary_click(Position<T> pointer) override;
  void process_secondary_click(Position<T> pointer) override;
  void process_confirm() override;
};
