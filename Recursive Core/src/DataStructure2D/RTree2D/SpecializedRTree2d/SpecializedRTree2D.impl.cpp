#include "SpecializedRTree2D.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class SpecializedRTree2D<BasicLongDouble>;
template class SpecializedRTree2D<_LongDoubleBitset>;
template class SpecializedRTree2D<_LongDoubleUInt64>;
