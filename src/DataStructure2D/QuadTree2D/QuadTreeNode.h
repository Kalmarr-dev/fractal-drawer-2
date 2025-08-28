#pragma once

#include <unordered_set>
#include "../../Shapes/IShape.h"

template<typename T>
struct QuadTreeNode {
  std::unordered_set<IShape<T>*> shapes_set;
  Position<T> lower;
  Position<T> higher;
  QuadTreeNode<T>* bottom_left;
  QuadTreeNode<T>* bottom_right;
  QuadTreeNode<T>* top_left;
  QuadTreeNode<T>* top_right;

  QuadTreeNode(Position<T> lower, Position<T> higher) : lower(lower), higher(higher) {
    bottom_left = nullptr;
    bottom_right = nullptr;
    top_left = nullptr;
    top_right = nullptr;
  }

  bool create_children_if_not_exist() {
    if (lower.x + (higher.x - lower.x) / T(2.0) == lower.x
          || lower.y + (higher.y - lower.y) / T(2.0) == lower.y
          || lower.x + (higher.x - lower.x) / T(2.0) == higher.x
          || lower.y + (higher.y - lower.y) / T(2.0) == higher.y)
    {
      return false;
    }
    
    Position<T> center(lower.x + (higher.x - lower.x) / T(2.0), lower.y + (higher.y - lower.y) / T(2.0));
    if (bottom_left == nullptr)
    {
      bottom_left = new QuadTreeNode<T>(lower, center);
    }
    if (bottom_right == nullptr)
    {
      bottom_right = new QuadTreeNode<T>(Position<T>(center.x, lower.y), Position<T>(higher.x, center.y));
    }
    if (top_left == nullptr)
    {
      top_left = new QuadTreeNode<T>(Position<T>(lower.x, center.y), Position<T>(center.x, higher.y));
    }
    if (top_right == nullptr)
    {
      top_right = new QuadTreeNode<T>(center, higher);
    }
    return true;
  }
  
  bool shape_is_inside(IShape<T>* shape) {
    for (auto &&point : shape->get_points())
    {
      if (point.x < lower.x || higher.x < point.x
          || point.y < lower.y || higher.y < point.y)
      {
        return false;
      }
    }
    return true;
  }

  Shapes<T> get_visible_shapes_in_area_recursive(Position<T> lower, Position<T> higher, double minimum_shape_size) {
    Shapes<T> result;
    
    if (this->higher.x - this->lower.x + this->higher.y - this->lower.y
          < (higher.x - lower.x + higher.y - lower.y) * T(minimum_shape_size))
    {
      return result;
    }
    
    if (this->intersects_with({this->lower, this->higher}, {lower, higher}))
    {
      for (auto &&shape : this->shapes_set)
      {
        result.add_shape(shape);
      }
      if (this->bottom_left != nullptr)
      {
        for (auto &&shape : this->bottom_left->get_visible_shapes_in_area_recursive(lower, higher, minimum_shape_size).get_shapes()) {
          result.add_shape(shape);
        }
      }
      if (this->bottom_right != nullptr)
      {
        for (auto &&shape : this->bottom_right->get_visible_shapes_in_area_recursive(lower, higher, minimum_shape_size).get_shapes()) {
          result.add_shape(shape);
        }
      }
      if (this->top_left != nullptr)
      {
        for (auto &&shape : this->top_left->get_visible_shapes_in_area_recursive(lower, higher, minimum_shape_size).get_shapes()) {
          result.add_shape(shape);
        }
      }
      if (this->top_right != nullptr)
      {
        for (auto &&shape : this->top_right->get_visible_shapes_in_area_recursive(lower, higher, minimum_shape_size).get_shapes()) {
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
    if (bottom_left != nullptr)
    {
      bottom_left->delete_children();
      delete bottom_left;
    }
    if (bottom_right != nullptr)
    {
      bottom_right->delete_children();
      delete bottom_right;
    }
    if (top_left != nullptr)
    {
      top_left->delete_children();
      delete top_left;
    }
    if (top_right != nullptr)
    {
      top_right->delete_children();
      delete top_right;
    }
  }
};
