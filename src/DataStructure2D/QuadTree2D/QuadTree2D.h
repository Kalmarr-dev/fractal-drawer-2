#pragma once

#include "../IDataStructure2D.h"
#include "../../Shapes/Shapes.h"
#include "../../Configuration/Configuration.h"
#include "QuadTreeNode.h"

template<typename T>
class QuadTree2D : public IDataStructure2D<T>
{
private:
  QuadTreeNode<T>* root;
  Configuration configuration;
  const unsigned int MIN_SHAPES_PER_NODE = 500; // before is spawns children
public:
  QuadTree2D(Position<T> a, Position<T> b, Configuration configuration);
  ~QuadTree2D();

  void add_shapes(Shapes<T> shapes) override;
  Shapes<T> get_visible_shapes_in_area(Position<T> lower, Position<T> higher) override;
  void clear_shapes() override;
};
