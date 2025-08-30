#pragma once

#include <list>
#include <map>
#include "../../../Shapes/Shapes.h"
#include "../../../Shapes/Line.h"
#include "../../../Shapes/IShape.fwd.h"
#include "../../../Camera/ICamera.h"
#include "../../../helpers/ScaleRotationMatrix.h"

template<typename T>
class Stamp
{
private:
  Line<T> root_line;
  T width;
  std::map<IShape<T>*, IShape<T>*> shapes_map;
  std::multimap<IShape<T>*, IShape<T>*> shapes_map_reverse;
  Shapes<T> new_shapes;
  int amount_of_shapes = 0;
  std::list<Stamp<T>*> inside_child_stamps;
  // only in root stamp
  std::list<Stamp<T>*> outside_child_stamps;
  int reflection_id = -1;
  int max_reflection_id = -1;
  T depth = 0.0;

  std::vector<Position<T>> get_corners();
  std::vector<Line<T>> get_lines();
  // bool is_fully_inside(Stamp<T>* stamp);
  Shapes<T> create_children_recursive
  (
    const std::vector<ScaleRotationMatrix<T>>& root_start_to_start_srms,
    const std::vector<ScaleRotationMatrix<T>>& start_to_end_srms,
    Stamp<T>* parent_stamp, ICamera<T>* p_camera,
    int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH,
    double* last_depth_exponent_numerator
  );
  std::vector<Stamp<T>*> get_deepest_stamps_containing_shape(IShape<T>* shape);
  IShape<T>* transform_shape_into_new_stamp_coordinates(Rectangle<T>* rectangle, Stamp<T>* stamp);
  Shapes<T> propagate_new_shape_recursive(IShape<T>* shape, Stamp<T>* root);
public:
  Stamp(Line<T> root_line, T width, Shapes<T> shapes_inside);
  ~Stamp() = default;
  Shapes<T> get_new_shapes();
  Shapes<T> get_lines_recursive();
  Line<T> get_root_line() {
    return root_line;
  }
  // creates new stamps recursively
  // then adds new shapes in a loop until no new shapes are added
  // use only on root
  Shapes<T> update_on_zoom(ICamera<T>* p_camera, int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH, double* last_depth_exponent_numerator);
  // use only on root
  Shapes<T> update_on_new_shapes(Shapes<T>& shapes_to_add);
  // Shapes<T> update_on_shapes_clear(Shapes<T> shapes_to_clear);
  bool stamp_is_inside(Stamp<T>* stamp);
  bool shape_is_inside(IShape<T>* shape);
  void clear();
  Stamp<T>* add_child_stamp(Line<T> root_line, T width);
  void add_child_stamp(Stamp<T>* stamp);
  // void remove_child_stamp(Stamp<T>* p_stamp);
  void set_depth(T depth);
};
