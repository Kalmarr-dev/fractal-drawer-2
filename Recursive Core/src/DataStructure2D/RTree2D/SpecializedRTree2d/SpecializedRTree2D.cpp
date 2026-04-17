#include "SpecializedRTree2D.h"

#ifndef NO_OMP
#include <omp.h>
#endif
#include <iostream>

template<typename T>
SpecializedRTree2D<T>::SpecializedRTree2D(Configuration configuration)
: configuration(configuration) {
  this->root = nullptr;
}

template<typename T>
SpecializedRTree2D<T>::~SpecializedRTree2D()
{
  if (this->root != nullptr)
  {
    this->root->delete_children();
    delete this->root;
  }
}

template<typename T>
void SpecializedRTree2D<T>::add_shapes(Shapes<T> shapes) {
  this->shapes_amount += shapes.get_shapes().size();
  std::cout << this->shapes_amount << '\n';
  // #pragma omp parallel for schedule(dynamic)
  for (auto &&shape : shapes.get_shapes())
  {
    bool inserted = false;
    if (root == nullptr)
    {
      root = new SpecializedRTreeNode<T>(shape);
      inserted = true;
    }

    if (!inserted)
    {
      std::pair<SpecializedRTreeNode<T>*, SpecializedRTreeNode<T>*> new_nodes = root->insert_recursive(new SpecializedRTreeNode<T>(shape));
      if (new_nodes.second != nullptr)
      {
        SpecializedRTreeNode<T>* new_root = new SpecializedRTreeNode<T>({0, 0}, {0, 0});
        new_root->children.push_back(new_nodes.first);
        new_root->children.push_back(new_nodes.second);
        new_root->recalculate_boundary();
        if (new_nodes.first != root && new_nodes.second != root)
        {
          delete root;
        }
        root = new_root;
      }
    }
  }
}

template<typename T>
Shapes<T> SpecializedRTree2D<T>::get_visible_shapes_in_area(Position<T> lower, Position<T> higher) {
  Shapes<T> result;

  if (root != nullptr)
  {
    root->get_visible_shapes_in_area_recursive(lower, higher, configuration.minimum_shape_size, result);
  }

  return result;
}

template<typename T>
void SpecializedRTree2D<T>::clear_shapes() {
  if (this->root != nullptr)
  {
    this->root->delete_children();
  }
  this->root->shape = nullptr;
  this->shapes_amount = 0;
}

template<typename T>
void SpecializedRTree2D<T>::clear_shapes(const Shapes<T>& shapes) {
  throw "SpecializedRTree2D<T>::clear_shapes not implemented";
  // this->shapes_amount -= shapes.get_shapes().size();
  // std::cout << this->shapes_amount << '\n';
  // #pragma omp parallel for schedule(dynamic)
  // for (auto &&shape : shapes.get_shapes())
  // {
  //   QuadTreeNode<T>* node = root;
  //   bool deleted = false;
  //   if (shape->get_linear_size_squared() == T(0.0))
  //   {
  //     #pragma omp critical(node_shapes)
  //     {
  //       node->shapes_set.erase(shape);
  //       deleted = true;
  //     }
  //   }

  //   while (!deleted && node->shape_is_inside(shape))
  //   {
  //     if (node->bottom_left->shape_is_inside(shape))
  //     {
  //       node = node->bottom_left;
  //     } else if (node->bottom_right->shape_is_inside(shape))
  //     {
  //       node = node->bottom_right;
  //     } else if (node->top_left->shape_is_inside(shape))
  //     {
  //       node = node->top_left;
  //     } else if (node->top_right->shape_is_inside(shape))
  //     {
  //       node = node->top_right;
  //     } else {
  //       #pragma omp critical(node_shapes)
  //       {
  //         node->shapes_set.erase(shape);
  //         deleted = true;
  //       }
  //     }
  //   }
  // }
}
