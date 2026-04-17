#pragma once

#include <DataStructure2D/IDataStructure2D.h>
#include <Shapes/Shapes.h>
#include <Configuration/Configuration.h>
#include "SpecializedRTreeNode.h"

template<typename T>
class SpecializedRTree2D : public IDataStructure2D<T>
{
private:
  SpecializedRTreeNode<T>* root;
  Configuration configuration;
  int shapes_amount = 0;
public:
  SpecializedRTree2D(Configuration configuration);
  ~SpecializedRTree2D();

  void add_shapes(Shapes<T> shapes) override;
  Shapes<T> get_visible_shapes_in_area(Position<T> lower, Position<T> higher) override;
  void clear_shapes() override;
  void clear_shapes(const Shapes<T>& shapes) override;
};
