#pragma once

#include "../../Shapes/Shapes.h"

template<typename T>
struct KDTreeNode {
  Shapes<T> shapes;
  KDTreeNode* left;
  KDTreeNode* right;
};
