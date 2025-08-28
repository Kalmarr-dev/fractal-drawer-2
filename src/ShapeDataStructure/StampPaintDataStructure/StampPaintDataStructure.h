#pragma once

#include <list>
#include "../IShapeDataStructure.h"
#include "../../Camera/ICamera.h"
#include "../../Configuration/Configuration.h"
#include "../../Input/IPointerMoveObserver.h"
#include "../../Input/IPointerUpObserver.h"
#include "../../Input/IZoomObserver.h"
#include "Figure/Figure.h"
#include "Figure/ScreenCoordinateSquares.h"

template<typename T>
class StampPaintDataStructure : public IShapeDataStructure<T>, public IPointerMoveObserver<T>, public IPointerUpObserver
{
private:
  // Stamp<T>* current_stamp;
  ScreenCoordinateSquares<T>* current_screen_coordinate_squares;
  // std::list<Stamp<T>*> all_stamps;
  std::list<Figure<T>*> all_figures;
  std::list<bool> figure_was_drawn;
  Shapes<T> new_shapes;
  Shapes<T> temporary_shapes;
  ICamera<T>* p_camera;
  Configuration configuration;
  double minimum_visible_screen_size;
  T camera_scale_change_to_recalculate_stamps;
  T previous_camera_scale;
  double brush_radius = 0.025;
  double square_size = 0.004;

  void update_temporary_shapes();
  void update_temporary_shapes(const Shapes<T>);
public:
  StampPaintDataStructure(ICamera<T>* p_camera, Configuration configuration);
  ~StampPaintDataStructure() = default;

  void clear_shapes() override;
  Shapes<T> clear_last_shapes() override;
  Shapes<T> get_new_shapes() override;
  Shapes<T> get_temporary_shapes() override;
  void process_pointer_move(Position<T> pointer) override;
  void process_primary_click(Position<T> pointer) override;
  void process_secondary_click(Position<T> pointer) override;
  void process_zoom(Position<T> pointer, T scale_value) override;
  void process_pointer_up() override;
};
