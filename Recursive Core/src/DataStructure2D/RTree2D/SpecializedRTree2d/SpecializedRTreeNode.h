#pragma once

#include <DataStructure2D/RTree2D/RTreeNode.h>

const int SPEC_MAX_CAP = 15;
const int SPEC_MIN_CAP = 7;

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

  std::pair<SpecializedRTreeNode<T>*, SpecializedRTreeNode<T>*> insert_recursive(SpecializedRTreeNode<T>* node) {
    if (this->shape != nullptr) // leaf or root
    {
      return {this, node};
    } else { // intermediate node or root
      T bad_score = this->children[0]->get_size_difference(node) + this->children[0]->get_potential_enlargement(node);
      SpecializedRTreeNode<T>* best_parent_node = this->children[0];
      for (auto &&child : this->children)
      {
        T child_bad_score = child->get_size_difference(node) + child->get_potential_enlargement(node);
        if (child_bad_score < bad_score)
        {
          bad_score = child_bad_score;
          best_parent_node = child;
        }
      }
      auto new_nodes = best_parent_node->insert_recursive(node);
      if (new_nodes.first != best_parent_node)
      {
        for (int i = 0; i < (int)this->children.size(); i++)
        {
          if (this->children[i] == best_parent_node)
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

  T get_size_difference(SpecializedRTreeNode<T>* node) {
    T linear_size = this->get_linear_size();
    T node_linear_size = std::max(higher.x - lower.x, higher.y - lower.y);
    T size_difference = linear_size - node_linear_size; // TODO do logarithm
    if (size_difference < T(0))
    {
      return T(-1.0) * size_difference;
    } else {
      return size_difference;
    }
  }

  T get_potential_enlargement(const SpecializedRTreeNode<T>* node) {
    Position<T> new_lower = lower;
    Position<T> new_higher = higher;
    new_lower.x = std::min(new_lower.x, node->lower.x);
    new_lower.y = std::min(new_lower.y, node->lower.y);
    new_higher.x = std::max(new_higher.x, node->higher.x);
    new_higher.y = std::max(new_higher.y, node->higher.y);
    T x_enlargement = new_higher.x - higher.x + lower.x - new_lower.x;
    T y_enlargement = new_higher.y - higher.y + lower.y - new_lower.y;
    return x_enlargement < y_enlargement ? y_enlargement : x_enlargement;
  }

  std::pair<SpecializedRTreeNode<T>*, SpecializedRTreeNode<T>*> split() {
    if (this->children.size() < 2)
    {
      throw "not enough children for split";
    }

    std::pair<SpecializedRTreeNode<T>*, SpecializedRTreeNode<T>*> principal_nodes;
    principal_nodes = {this->children[0], this->children[1]};
    T principal_nodes_score = 0;
    for (int i = 0; i < (int)this->children.size(); i++)
    {
      SpecializedRTreeNode<T>* child1 = this->children[i];
      for (int j = i + 1; j < (int)this->children.size(); j++)
      {
        SpecializedRTreeNode<T>* child2 = this->children[j];
        Position<T> lower;
        lower.x = std::min(child1->lower.x, child2->lower.x);
        lower.y = std::min(child1->lower.y, child2->lower.y);
        Position<T> higher;
        higher.x = std::max(child1->higher.x, child2->higher.x);
        higher.y = std::max(child1->higher.y, child2->higher.y);
        T score = (higher.x - lower.x) * (higher.y - lower.y) - (std::max(child1->get_area(), child2->get_area()));
        if (principal_nodes_score < score)
        {
          principal_nodes = {child1, child2};
        }
      }
    }

    SpecializedRTreeNode<T>* new_node1 = new SpecializedRTreeNode<T>({0, 0}, {0, 0});
    new_node1->children.push_back(principal_nodes.first);
    SpecializedRTreeNode<T>* new_node2 = new SpecializedRTreeNode<T>({0, 0}, {0, 0});
    new_node2->children.push_back(principal_nodes.second);
    for (int i = 0; i < (int)this->children.size(); i++) {
      SpecializedRTreeNode<T>* child = this->children[i];

      if (child == principal_nodes.first || child == principal_nodes.second)
      {
        continue;
      }

      if (new_node1->children.size() >= SPEC_MIN_CAP && new_node2->children.size() < SPEC_MIN_CAP)
      {
        new_node2->children.push_back(child);
        continue;
      } else if (new_node1->children.size() < SPEC_MIN_CAP && new_node2->children.size() >= SPEC_MIN_CAP) {
        new_node1->children.push_back(child);
        continue;
      }
      
      T bad_score1 = new_node1->get_potential_enlargement(child) + new_node1->get_size_difference(child);
      T bad_score2 = new_node2->get_potential_enlargement(child) + new_node2->get_size_difference(child);
      if (bad_score1 < bad_score2)
      {
        new_node1->children.push_back(child);
      } else {
        new_node2->children.push_back(child);
      }
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