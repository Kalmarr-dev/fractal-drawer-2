#include "RTree2D.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class RTree2D<BasicLongDouble>;
template class RTree2D<_LongDoubleBitset>;
template class RTree2D<_LongDoubleUInt64>;
