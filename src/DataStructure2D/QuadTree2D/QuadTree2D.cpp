#include "QuadTree2D.h"

#include <omp.h>

template<typename T>
QuadTree2D<T>::QuadTree2D(Position<T> a, Position<T> b, Configuration configuration)
: configuration(configuration)
{
  if (b.x < a.x)
  {
    std::swap(a.x, b.x);
  }
  if (b.y < a.y)
  {
    std::swap(a.y, b.y);
  }
  root = new QuadTreeNode<T>(a, b);
}

template<typename T>
QuadTree2D<T>::~QuadTree2D()
{
  if (this->root != nullptr)
  {
    this->root->delete_children();
    delete this->root;
  }
}

template<typename T>
void QuadTree2D<T>::add_shapes(Shapes<T> shapes) {
  #pragma omp parallel for schedule(dynamic)
  for (auto &&shape : shapes.get_shapes())
  {
    QuadTreeNode<T>* node = root;
    bool inserted = false;

    while (!inserted && node->shape_is_inside(shape))
    {
      #pragma omp critical(node_shapes)
      {
        if (node->shapes.get_shapes().size() < MIN_SHAPES_PER_NODE) {
          node->shapes.add_shape(shape);
          inserted = true;
        }
      }
      if (!inserted)
      {
        node->create_children_if_not_exist();
        if (node->bottom_left->shape_is_inside(shape))
        {
          node = node->bottom_left;
        } else if (node->bottom_right->shape_is_inside(shape))
        {
          node = node->bottom_right;
        } else if (node->top_left->shape_is_inside(shape))
        {
          node = node->top_left;
        } else if (node->top_right->shape_is_inside(shape))
        {
          node = node->top_right;
        } else {
          #pragma omp critical(node_shapes)
          {
            node->shapes.add_shape(shape);
            inserted = true;
          }
        }
      }
    }
  }
}

template<typename T>
Shapes<T> QuadTree2D<T>::get_visible_shapes_in_area(Position<T> lower, Position<T> higher) {
  QuadTreeNode<T>* node = root;
  Shapes<T> result;
  
  for (auto &&shape : node->shapes.get_shapes())
  {
    result.add_shape(shape);
  }

  for (auto &&shape : node->get_visible_shapes_in_area_recursive(lower, higher, configuration.minimum_shape_size).get_shapes())
  {
    result.add_shape(shape);
  }

  return result;
}

template<typename T>
void QuadTree2D<T>::clear_shapes() {
  if (this->root != nullptr)
  {
    this->root->delete_children();
  }
  root->shapes.clear();
}

