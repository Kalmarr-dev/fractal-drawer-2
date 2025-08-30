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
  int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH,
  double* last_depth_exponent_numerator
) {
  if (
    this->root_line.get_linear_size_squared() < MIN_LINE_SIZE * MIN_LINE_SIZE
    ||  width * width < MIN_LINE_SIZE * MIN_LINE_SIZE
  )
  {
    return Shapes<T>();
  }
  Shapes<T> new_stamp_lines;
  // TODO return if not on camera
  // TODO consider max_depth
  // TODO consider maxlines
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
      new_stamp->set_depth(++*last_depth_exponent_numerator);
      this->inside_child_stamps.push_back(new_stamp);
      for (auto &&line : new_stamp->get_lines())
      {
        Line<T>* p_line = new Line(line);
        p_line->set_depth(new_stamp->depth);
        new_stamp_lines.add_shape(p_line);
      }
      child = new_stamp;
    }
    i++;
  }
  for (auto &&child : this->inside_child_stamps)
  {
    Shapes<T> deeper_shapes = child->create_children_recursive(
      root_start_to_start_srms, start_to_end_srms, this,
      p_camera, MAXLINES, MIN_LINE_SIZE, MAX_DEPTH,
      last_depth_exponent_numerator
    );
    for (auto &&line : deeper_shapes.get_shapes())
    {
      new_stamp_lines.add_shape(line);
    }
  }
  
  return new_stamp_lines;
}

template<typename T>
std::vector<Stamp<T>*> Stamp<T>::get_deepest_stamps_containing_shape(IShape<T>* shape) {
  if (!this->shape_is_inside(shape))
  {
    return std::vector<Stamp<T>*>();
  }
  std::vector<Stamp<T>*> deepest_stamps;
  bool child_contains_this_shape = false;
  for (auto &&child : this->inside_child_stamps) {
    std::vector<Stamp<T>*> deepest_stamps_to_add = child->get_deepest_stamps_containing_shape(shape);
    for (auto &&stamp : deepest_stamps_to_add)
    {
      child_contains_this_shape = true;
      deepest_stamps.push_back(stamp);
    }
  }
  for (auto &&child : this->outside_child_stamps) {
    std::vector<Stamp<T>*> deepest_stamps_to_add = child->get_deepest_stamps_containing_shape(shape);
    for (auto &&stamp : deepest_stamps_to_add)
    {
      child_contains_this_shape = true;
      deepest_stamps.push_back(stamp);
    }
  }
  if (!child_contains_this_shape)
  {
    deepest_stamps.push_back(this);
  }
  return deepest_stamps;
}

template<typename T>
IShape<T>* Stamp<T>::transform_shape_into_new_stamp_coordinates(Rectangle<T>* rectangle, Stamp<T>* stamp) {
  // TODO support rotation
  auto point = rectangle->get_points()[0];
  T start_to_point_x = (point.x - this->root_line.a.x) / this->width * stamp->width;
  T start_to_point_y = (point.y - this->root_line.a.y) / (this->root_line.b.y - this->root_line.a.y) * (stamp->root_line.b.y - stamp->root_line.a.y);
  T x = stamp->root_line.a.x + start_to_point_x;
  T y = stamp->root_line.a.y + start_to_point_y;
  T min_x = x;
  T max_x = x;
  T min_y = y;
  T max_y = y;
  for (auto &&point : rectangle->get_points())
  {
    T start_to_point_x = (point.x - this->root_line.a.x) / this->width * stamp->width;
    T start_to_point_y = (point.y - this->root_line.a.y) / (this->root_line.b.y - this->root_line.a.y) * (stamp->root_line.b.y - stamp->root_line.a.y);
    T x = stamp->root_line.a.x + start_to_point_x;
    T y = stamp->root_line.a.y + start_to_point_y;
    min_x = std::min(min_x, x);
    max_x = std::max(max_x, x);
    min_y = std::min(min_y, y);
    max_y = std::max(max_y, y);
  }
  return new Rectangle<T>(Position<T>(min_x, min_y), Position<T>(max_x, max_y));
}

template<typename T>
Shapes<T> Stamp<T>::propagate_new_shape_recursive(IShape<T>* root_shape, Stamp<T>* root) {
  Shapes<T> new_shapes;
  IShape<T>* transformed_shape = nullptr;
  if (root_shape->get_type() == ShapeType::RECTANGLE)
  {
    Rectangle<T>* rectangle = dynamic_cast<Rectangle<T>*>(root_shape);
    transformed_shape = root->transform_shape_into_new_stamp_coordinates(rectangle, this);
    transformed_shape->set_depth(root_shape->get_depth());
    this->shapes_map[transformed_shape] = root_shape;
    this->shapes_map_reverse.insert(std::make_pair(root_shape, transformed_shape));
    new_shapes.add_shape(transformed_shape);
  } else {
    throw std::runtime_error("Not rectangle in Stamp::propagate_new_shape_recursive");
  }
  for (auto &&child : this->inside_child_stamps) {
    for (auto &&shape : child->propagate_new_shape_recursive(root_shape, root).get_shapes())
    {
      new_shapes.add_shape(shape);
    }
  }
  for (auto &&child : this->outside_child_stamps) {
    for (auto &&shape : child->propagate_new_shape_recursive(root_shape, root).get_shapes())
    {
      new_shapes.add_shape(shape);
    }
  }
  return new_shapes;
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
Shapes<T> Stamp<T>::update_on_zoom(ICamera<T>* p_camera, int MAXLINES, T MIN_LINE_SIZE, int MAX_DEPTH, double* last_depth_exponent_numerator) {
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
      root_start_to_start_srms, start_to_end_srms, this, p_camera, MAXLINES, MIN_LINE_SIZE, MAX_DEPTH, last_depth_exponent_numerator
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
      root_start_to_start_srms, start_to_end_srms, this, p_camera, MAXLINES, MIN_LINE_SIZE, MAX_DEPTH, last_depth_exponent_numerator
    );
    for (auto &&shape : stamp_shapes.get_shapes())
    {
      shape->set_depth(this->depth);
      new_shapes.add_shape(shape);
    }
  }
  // Add shapes from root in new Stamps
  // TODO

  return new_shapes;
}

template<typename T>
Shapes<T> Stamp<T>::update_on_new_shapes(Shapes<T>& shapes_to_add) {
  Shapes<T> new_shapes;
  // TODO pragma omp parallel for, critical region
  for (auto &&shape : shapes_to_add.get_shapes())
  {
    // find out in which deepest stamps these new shapes are
    std::vector<Stamp<T>*> deepest_stamps(this->get_deepest_stamps_containing_shape(shape));
    // update root stamp with all the shapes transformed from the shapes in stamps
    Shapes<T> new_root_shapes;
    for (auto &&stamp : deepest_stamps)
    {
      if (shape->get_type() == ShapeType::RECTANGLE)
      {
        Rectangle<T>* rectangle = dynamic_cast<Rectangle<T>*>(shape);
        rectangle = dynamic_cast<Rectangle<T>*>(stamp->transform_shape_into_new_stamp_coordinates(rectangle, this));
        rectangle->set_depth(shape->get_depth());
        new_root_shapes.add_shape(rectangle);
      } else {
        throw std::runtime_error("Not rectangle in Stamp::update_on_new_shapes");
      }
    }
    // propagate updated shapes down, assigning depth from the stamps
    for (auto &&root_shape : new_root_shapes.get_shapes())
    {
      for (auto &&shape : this->propagate_new_shape_recursive(root_shape, this).get_shapes())
      {
        new_shapes.add_shape(shape);
      }
    }
  }
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
