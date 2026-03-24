#pragma once

#include "../IDataStructure2D.h"
#include <Shapes/Shapes.h>
#include <Configuration/Configuration.h>
#include "RTreeNode.h"

template<typename T>
class RTree2D : public IDataStructure2D<T>
{
private:
  RTreeNode<T>* root;
  Configuration configuration;
  int shapes_amount = 0;
public:
  RTree2D(Configuration configuration);
  ~RTree2D();

  void add_shapes(Shapes<T> shapes) override;
  Shapes<T> get_visible_shapes_in_area(Position<T> lower, Position<T> higher) override;
  void clear_shapes() override;
  void clear_shapes(const Shapes<T>& shapes) override;
};
