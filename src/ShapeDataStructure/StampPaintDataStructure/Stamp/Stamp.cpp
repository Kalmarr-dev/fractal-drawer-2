#include "Stamp.h"

#include <cmath>

template<typename T>
Stamp<T>::Stamp(Line<T> root_line, T width, Shapes<T> shapes_inside) 
  : root_line(root_line), width(width)
{
  this->amount_of_shapes += shapes_inside.get_shapes().size();
  for (auto &&shape : shapes_inside.get_shapes())
  {
    this->shapes_map[shape] = shape;
    this->shapes_map_reverse.insert(std::make_pair(shape, shape));
  }
}

template<typename T>
std::vector<Position<T>> Stamp<T>::get_corners() {
  // TODO support line rotation
  std::vector<Position<T>> corners;
  corners.emplace_back(this->root_line.a);
  corners.emplace_back(this->root_line.a.x + this->width, this->root_line.a.y);
  corners.emplace_back(this->root_line.b.x + this->width, this->root_line.b.y);
  corners.emplace_back(this->root_line.b);
  return corners;
}

template<typename T>
std::vector<Line<T>> Stamp<T>::get_lines() {
  auto corners = this->get_corners();
  std::vector<Line<T>> lines;
  for (int i = 1; i < (int)corners.size(); i++)
  {
    lines.emplace_back(corners[i - 1], corners[i]);
  }
  lines.emplace_back(corners[corners.size() - 1], corners[0]);
  return lines;
}

template<typename T>
Shapes<T> Stamp<T>::create_children_recursive
(
  const std::vector<ScaleRotationMatrix<T>>& root_start_to_start_srms,
  const std::vector<ScaleRotationMatrix<T>>& start_to_end_srms,
  Stamp<T>* parent_stamp, ICamera<T>* p_camera,
  int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH
) {
  if (
    this->root_line.get_linear_size_squared() < MIN_LINE_SIZE * MIN_LINE_SIZE
    ||  width < MIN_LINE_SIZE
  )
  {
    return Shapes<T>();
  }
  Shapes<T> new_stamp_lines;
  // TODO return if not on camera
  // TODO consider max_depth
  int i = 0;
  for (auto &&sibling_stamp : parent_stamp->inside_child_stamps)
  {
    Stamp<T>* child = nullptr;
    for (auto &&child_stamp : this->inside_child_stamps)
    {
      if (child_stamp->reflection_id == sibling_stamp->reflection_id)
      {
        child = child_stamp;
        break;
      }
    }
    if (child == nullptr)
    {
      T start_to_start_x = (sibling_stamp->root_line.a.x - parent_stamp->root_line.a.x) / parent_stamp->width * this->width;
      Line<T> start_to_start_vector = root_start_to_start_srms[i].MultiplyByVector(this->root_line);
      T start_to_start_y = start_to_start_vector.b.y;
      Line<T> child_root_vector = start_to_end_srms[i].MultiplyByVector(this->root_line);
      T x0, y0, x1, y1;
      x0 = this->root_line.a.x + start_to_start_x;
      y0 = this->root_line.a.y + start_to_start_y;
      x1 = x0 + child_root_vector.b.x;
      y1 = y0 + child_root_vector.b.y;
      T new_width = sibling_stamp->width * this->width / parent_stamp->width;
      Line<T> new_root_line(Position<T>(x0, y0), Position<T>(x1, y1));
      if (this->root_line.get_linear_size_squared() < new_root_line.get_linear_size_squared())
      {
        i++;
        continue;
      }
      Stamp<T>* new_stamp = new Stamp<T>(new_root_line, new_width, Shapes<T>());
      new_stamp->reflection_id = sibling_stamp->reflection_id;
      this->inside_child_stamps.push_back(new_stamp);
      for (auto &&line : new_stamp->get_lines())
      {
        Line<T>* p_line = new Line(line);
        // TODO set depth
        new_stamp_lines.add_shape(p_line);
      }
      child = new_stamp;
    }    
    Shapes<T> deeper_shapes = child->create_children_recursive(
      root_start_to_start_srms, start_to_end_srms, this,
      p_camera, MAXLINES, MIN_LINE_SIZE, MAX_DEPTH
    );
    for (auto &&line : deeper_shapes.get_shapes())
    {
      new_stamp_lines.add_shape(line);
    }
    i++;
  }
  return new_stamp_lines;
}

template<typename T>
Shapes<T> Stamp<T>::get_lines_recursive() {
  Shapes<T> shapes;
  for (auto &&line : this->get_lines())
  {
    auto p_line = new Line<T>(line);
    p_line->set_depth(this->depth);
    shapes.add_shape(p_line);
  }
  for (auto &&stamp : inside_child_stamps)
  {
    for (auto &&shape : stamp->get_lines_recursive().get_shapes())
    {
      shape->set_depth(this->depth);
      shapes.add_shape(shape);
    }
  }
  for (auto &&stamp : outside_child_stamps)
  {
    for (auto &&shape : stamp->get_lines_recursive().get_shapes())
    {
      shape->set_depth(this->depth);
      shapes.add_shape(shape);
    }
  }
  return shapes;
}

template<typename T>
Shapes<T> Stamp<T>::update_on_zoom(ICamera<T>* p_camera, int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH) {
  // Create stamps that on camera, not too small and are reflections of inside_child_stamps
  std::vector<ScaleRotationMatrix<T>> root_start_to_start_srms;
  std::vector<ScaleRotationMatrix<T>> start_to_end_srms;
  Shapes<T> new_shapes;
  for (auto &&stamp : inside_child_stamps)
  {
    Line<T> start_to_stamp_start_line = Line<T>(this->root_line.a, stamp->root_line.a);
    root_start_to_start_srms.emplace_back(this->root_line, start_to_stamp_start_line);
    start_to_end_srms.emplace_back(this->root_line, stamp->root_line);
  }
  for (auto &&stamp : inside_child_stamps)
  {
    Shapes<T> stamp_shapes = stamp->create_children_recursive(
      root_start_to_start_srms, start_to_end_srms, this, p_camera, MAXLINES, MIN_LINE_SIZE, MAX_DEPTH
    );
    for (auto &&shape : stamp_shapes.get_shapes())
    {
      shape->set_depth(this->depth);
      new_shapes.add_shape(shape);
    }
  }
  for (auto &&stamp : outside_child_stamps)
  {
    Shapes<T> stamp_shapes = stamp->create_children_recursive(
      root_start_to_start_srms, start_to_end_srms, this, p_camera, MAXLINES, MIN_LINE_SIZE, MAX_DEPTH
    );
    for (auto &&shape : stamp_shapes.get_shapes())
    {
      shape->set_depth(this->depth);
      new_shapes.add_shape(shape);
    }
  }
  // Add new shapes bottom-to-top
  // TODO

  return new_shapes;
}

template<typename T>
bool Stamp<T>::stamp_is_inside(Stamp<T>* stamp) {
  // TODO support rotated stamp
  auto corners = this->get_corners();
  T min_x = corners[0].x;
  T max_x = corners[0].x;
  T min_y = corners[0].y;
  T max_y = corners[0].y;
  for (auto &&corner : corners)
  {
    min_x = std::min(min_x, corner.x);
    max_x = std::max(max_x, corner.x);
    min_y = std::min(min_y, corner.y);
    max_y = std::max(max_y, corner.y);
  }
  for (auto &&point : stamp->get_corners())
  {
    if (point.x < min_x || max_x < point.x
        || point.y < min_y || max_y < point.y)
    {
      return false;
    }
  }
  return true;
}

template<typename T>
bool Stamp<T>::shape_is_inside(IShape<T>* shape) {
  // TODO support rotated stamp
  auto corners = this->get_corners();
  T min_x = corners[0].x;
  T max_x = corners[0].x;
  T min_y = corners[0].y;
  T max_y = corners[0].y;
  for (auto &&corner : corners)
  {
    min_x = std::min(min_x, corner.x);
    max_x = std::max(max_x, corner.x);
    min_y = std::min(min_y, corner.y);
    max_y = std::max(max_y, corner.y);
  }
  for (auto &&point : shape->get_points())
  {
    if (point.x < min_x || max_x < point.x
        || point.y < min_y || max_y < point.y)
    {
      return false;
    }
  }
  return true;
}

template<typename T>
Stamp<T>* Stamp<T>::add_child_stamp(Line<T> root_line, T width) {
  Stamp<T>* child = new Stamp<T>(root_line, width, Shapes<T>());
  add_child_stamp(child);
  return child;
}

template<typename T>
void Stamp<T>::add_child_stamp(Stamp<T>* child) {
  if (stamp_is_inside(child))
  {
    inside_child_stamps.push_back(child);
    child->reflection_id = ++this->max_reflection_id;
  } else {
    outside_child_stamps.push_back(child);
  }
}

template<typename T>
void Stamp<T>::set_depth(T depth) {
  this->depth = depth;
}
