#pragma once

#include <DataStructure2D/RTree2D/RTreeNode.h>

const int SPEC_MAX_CAP = 3;
// const int SPEC_MIN_CAP = 1;

template<typename T>
struct SpecializedRTreeNode {
#ifndef NO_OMP
  omp_lock_t shapes_set_lock;
  omp_lock_t create_children_lock;
#endif
  Position<T> lower;
  Position<T> higher;
  IShape<T>* shape;
  std::vector<SpecializedRTreeNode<T>*> children;

  SpecializedRTreeNode(Position<T> lower, Position<T> higher) : lower(lower), higher(higher), shape(nullptr) {
#ifndef NO_OMP
    omp_init_lock(&shapes_set_lock);
    omp_init_lock(&create_children_lock);
#endif
  }

  SpecializedRTreeNode(IShape<T>* shape) : shape(shape) {
    auto points = shape->get_points();
    lower.x = points[0].x;
    higher.x = points[0].x;
    lower.y = points[0].y;
    higher.y = points[0].y;
    for (const auto &point : points)
    {
      if (point.x < lower.x) {
        lower.x = point.x;
      }
      if (point.y < lower.y) {
        lower.y = point.y;
      }
      if (higher.x < point.x)
      {
        higher.x = point.x;
      }
      if (higher.y < point.y)
      {
        higher.y = point.y;
      }
    }
#ifndef NO_OMP
    omp_init_lock(&shapes_set_lock);
    omp_init_lock(&create_children_lock);
#endif
  }

  ~SpecializedRTreeNode() {
#ifndef NO_OMP
    omp_destroy_lock(&shapes_set_lock);
    omp_destroy_lock(&create_children_lock);
#endif
  }

  std::pair<SpecializedRTreeNode<T>*, SpecializedRTreeNode<T>*> insert_shape_recursive(IShape<T>* shape) {
    if (this->shape != nullptr) // leaf or root
    {
      return {this, new SpecializedRTreeNode<T>(shape)};
    } else { // intermediate node or root
      T size_difference = this->children[0]->get_size_difference(shape);
      SpecializedRTreeNode<T>* similar_size_node = this->children[0];
      for (auto &&child : this->children)
      {
        T child_size_difference = child->get_size_difference(shape);
        if (child_size_difference < size_difference)
        {
          size_difference = child_size_difference;
          similar_size_node = child;
        }
      }
      auto new_nodes = similar_size_node->insert_shape_recursive(shape);
      if (new_nodes.first != similar_size_node)
      {
        for (int i = 0; i < (int)this->children.size(); i++)
        {
          if (this->children[i] == similar_size_node)
          {
            delete this->children[i];
            this->children[i] = new_nodes.first;
          }
        }
      }
      if (new_nodes.second != nullptr) {
        this->children.push_back(new_nodes.second);
      }
      std::pair<SpecializedRTreeNode<T>*, SpecializedRTreeNode<T>*> result = {nullptr, nullptr};
      if (SPEC_MAX_CAP < this->children.size())
      {
        result = this->split();
      } else {
        this->recalculate_boundary();
        result = {this, nullptr};
      }
      return result;
    }
  }

  T get_size_difference(IShape<T>* shape) {
    T linear_size = this->get_linear_size();
    Position<T> lower = shape->get_points()[0];
    Position<T> higher = shape->get_points()[0];
    for (const auto &point : shape->get_points())
    {
      lower.x = std::min(lower.x, point.x);
      lower.y = std::min(lower.y, point.y);
      higher.x = std::min(higher.x, point.x);
      higher.y = std::min(higher.y, point.y);
    }
    T shape_linear_size = std::max(higher.x - lower.x, higher.y - lower.y);
    T size_difference = linear_size - shape_linear_size; // TODO do logarithm
    if (size_difference < T(0))
    {
      return T(-1.0) * size_difference;
    } else {
      return size_difference;
    }
  }

  std::pair<SpecializedRTreeNode<T>*, SpecializedRTreeNode<T>*> split() {
    if (this->children.size() < 2)
    {
      throw "not enough children for split";
    }

    std::sort(
      this->children.begin(),
      this->children.end(),
      [](SpecializedRTreeNode<T>* lhs, SpecializedRTreeNode<T>* rhs) {
        T linear_size_lhs = lhs->get_linear_size();
        T linear_size_rhs = rhs->get_linear_size();
        return linear_size_lhs < linear_size_rhs;
    });

    int pivot = ((int)this->children.size()) / 2;


    SpecializedRTreeNode<T>* new_node1 = new SpecializedRTreeNode<T>({0, 0}, {0, 0});
    for (int i = 0; i < pivot; i++)
    {
      new_node1->children.push_back(this->children[i]);
    }
    SpecializedRTreeNode<T>* new_node2 = new SpecializedRTreeNode<T>({0, 0}, {0, 0});
    for (int i = pivot; i < (int)this->children.size(); i++)
    {
      new_node2->children.push_back(this->children[i]);
    }

    new_node1->recalculate_boundary();
    new_node2->recalculate_boundary();
    return {new_node1, new_node2};
  }

  T get_linear_size() const {
    return std::max(higher.x - lower.x, higher.y - lower.y);
  }

  T get_area() {
    return (higher.x - lower.x) * (higher.y - lower.y);
  }

  void recalculate_boundary() {
    lower.x = children[0]->lower.x;
    lower.y = children[0]->lower.y;
    higher.x = children[0]->higher.x;
    higher.y = children[0]->higher.y;
    
    for (auto &&child : children)
    {
      lower.x = std::min(lower.x, child->lower.x);
      lower.y = std::min(lower.y, child->lower.y);
      higher.x = std::max(higher.x, child->higher.x);
      higher.y = std::max(higher.y, child->higher.y);
    }
  }

  void get_visible_shapes_in_area_recursive(const Position<T>& lower, const Position<T>& higher, double minimum_shape_size, Shapes<T>& result) {    
    if (this->higher.x - this->lower.x + this->higher.y - this->lower.y
          < (higher.x - lower.x + higher.y - lower.y) * T(minimum_shape_size))
    {
      return;
    }
    
    if (intersects_with({this->lower, this->higher}, {lower, higher}))
    {
      if (this->shape)
      {
        result.add_shape(shape);
      }
      for (auto &&child : this->children)
      {
        child->get_visible_shapes_in_area_recursive(lower, higher, minimum_shape_size, result);
      }
    }

    return;
  }

  static bool intersects_with(std::pair<Position<T>, Position<T>> sorted_corners, std::pair<Position<T>, Position<T>> corners) {
    T min_x = sorted_corners.first.x;
    T max_x = sorted_corners.second.x;
    T min_y = sorted_corners.first.y;
    T max_y = sorted_corners.second.y;
    if (
      max_x < std::min(corners.first.x, corners.second.x)
      || std::max(corners.first.x, corners.second.x) < min_x
      || max_y < std::min(corners.first.y, corners.second.y)
      || std::max(corners.first.y, corners.second.y) < min_y
    ) {
      return false;
    } else {
      return true;
    }
  }

  void delete_children() {
    for (auto &&child : this->children)
    {
      child->delete_children();
      delete child;
    }
  }
};
