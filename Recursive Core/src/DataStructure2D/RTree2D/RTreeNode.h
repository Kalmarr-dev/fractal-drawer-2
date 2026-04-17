#pragma once

#include <unordered_set>
#include <set>
#include <Shapes/IShape.h>
#ifndef NO_OMP
#include <omp.h>
#endif

const int MAX_CAP = 3;
const int MIN_CAP = 1;

template<typename T>
struct RTreeNode {
#ifndef NO_OMP
  omp_lock_t shapes_set_lock;
  omp_lock_t create_children_lock;
#endif
  Position<T> lower;
  Position<T> higher;
  IShape<T>* shape;
  std::vector<RTreeNode<T>*> children;

  RTreeNode(Position<T> lower, Position<T> higher) : lower(lower), higher(higher), shape(nullptr) {
#ifndef NO_OMP
    omp_init_lock(&shapes_set_lock);
    omp_init_lock(&create_children_lock);
#endif
  }

  RTreeNode(IShape<T>* shape) : shape(shape) {
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

  ~RTreeNode() {
#ifndef NO_OMP
    omp_destroy_lock(&shapes_set_lock);
    omp_destroy_lock(&create_children_lock);
#endif
  }

  std::pair<RTreeNode<T>*, RTreeNode<T>*> insert_shape_recursive(IShape<T>* shape) {
    if (this->shape != nullptr) // leaf or root
    {
      return {this, new RTreeNode<T>(shape)};
    } else { // intermediate node or root
      T min_enlargement = 99999999;
      RTreeNode<T>* min_enlargement_node = nullptr;
      for (auto &&child : this->children)
      {
        T child_enlargement_after_adding_shape = child->get_potential_enlargement(shape);
        if (min_enlargement_node == nullptr
          || child_enlargement_after_adding_shape < min_enlargement)
        {
          min_enlargement = child_enlargement_after_adding_shape;
          min_enlargement_node = child;
        }
      }
      auto new_nodes = min_enlargement_node->insert_shape_recursive(shape);
      if (new_nodes.first != min_enlargement_node)
      {
        for (int i = 0; i < (int)this->children.size(); i++)
        {
          if (this->children[i] == min_enlargement_node)
          {
            delete this->children[i];
            this->children[i] = new_nodes.first;
          }
        }
      }
      if (new_nodes.second != nullptr) {
        this->children.push_back(new_nodes.second);
      }
      std::pair<RTreeNode<T>*, RTreeNode<T>*> result = {nullptr, nullptr};
      if (MAX_CAP < this->children.size())
      {
        result = this->split();
      } else {
        this->recalculate_boundary();
        result = {this, nullptr};
      }
      return result;
    }
  }
  
  T get_potential_enlargement(const IShape<T>* shape) {
    Position<T> new_lower = lower;
    Position<T> new_higher = higher;
    for (const auto &point : shape->get_points())
    {
      new_lower.x = std::min(new_lower.x, point.x);
      new_lower.y = std::min(new_lower.y, point.y);
      new_higher.x = std::max(new_higher.x, point.x);
      new_higher.y = std::max(new_higher.y, point.y);
    }
    T x_enlargement = new_higher.x - higher.x + lower.x - new_lower.x;
    T y_enlargement = new_higher.y - higher.y + lower.y - new_lower.y;
    return x_enlargement < y_enlargement ? y_enlargement : x_enlargement;
  }

  std::pair<RTreeNode<T>*, RTreeNode<T>*> split() {
    if (this->children.size() < 2)
    {
      throw "not enough children for split";
    }

    std::pair<RTreeNode<T>*, RTreeNode<T>*> principal_nodes;
    principal_nodes = {this->children[0], this->children[1]};
    T principal_nodes_score = 0;
    for (int i = 0; i < (int)this->children.size(); i++)
    {
      RTreeNode<T>* child1 = this->children[i];
      for (int j = i + 1; j < (int)this->children.size(); j++)
      {
        RTreeNode<T>* child2 = this->children[j];
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

    RTreeNode<T>* new_node1 = new RTreeNode<T>({0, 0}, {0, 0});
    new_node1->children.push_back(principal_nodes.first);
    RTreeNode<T>* new_node2 = new RTreeNode<T>({0, 0}, {0, 0});
    new_node2->children.push_back(principal_nodes.second);
    for (int i = 0; i < (int)this->children.size(); i++) {
      RTreeNode<T>* child = this->children[i];
      if (new_node1->children.size() >= MIN_CAP && new_node2->children.size() < MIN_CAP)
      {
        new_node2->children.push_back(child);
        continue;
      } else if (new_node1->children.size() < MIN_CAP && new_node2->children.size() >= MIN_CAP) {
        new_node1->children.push_back(child);
        continue;
      }
      
      if (child != principal_nodes.first && child != principal_nodes.second)
      {
        if (rand() % 2)
        {
          new_node1->children.push_back(child);
        } else {
          new_node2->children.push_back(child);
        }
      }
    }

    new_node1->recalculate_boundary();
    new_node2->recalculate_boundary();
    return {new_node1, new_node2};
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

  Shapes<T> get_visible_shapes_in_area_recursive(const Position<T>& lower, const Position<T>& higher, double minimum_shape_size) {
    Shapes<T> result;
    
    if (this->higher.x - this->lower.x + this->higher.y - this->lower.y
          < (higher.x - lower.x + higher.y - lower.y) * T(minimum_shape_size))
    {
      return result;
    }
    
    if (intersects_with({this->lower, this->higher}, {lower, higher}))
    {
      if (this->shape)
      {
        result.add_shape(shape);
      }
      for (auto &&child : this->children)
      {
        for (auto &&shape : child->get_visible_shapes_in_area_recursive(lower, higher, minimum_shape_size).get_shapes()) {
          result.add_shape(shape);
        }
      }
    }

    return result;
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
