#pragma once

#include <vector>
#include "../IDataStructure2D.h"
#include <Shapes/Shapes.h>
#include <Shapes/IShape.h>
#include "KDTreeNode.h"

template<typename T>
class KDTree2D : public IDataStructure2D<T>
{
private:
  std::vector<KDTreeNode<T>> nodes;

  bool shape_is_inside(Position<T> corner1, Position<T> corner2, IShape<T>* p_shape);
public:
  KDTree2D();
  ~KDTree2D();

  void add_shapes(Shapes<T> shapes) override;
  Shapes<T> get_visible_shapes_in_area(Position<T> lower, Position<T> higher) override;
  void clear_shapes() override;
};
