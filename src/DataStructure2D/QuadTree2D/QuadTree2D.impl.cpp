#include "QuadTree2D.cpp"

#include "../../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../../LongDouble/LongDoubleBitset/LongDoubleBitset.h"
#include "../../LongDouble/LongDoubleUInt64/LongDoubleUInt64.h"

template class QuadTree2D<BasicLongDouble>;
template class QuadTree2D<_LongDoubleBitset>;
template class QuadTree2D<_LongDoubleUInt64>;
