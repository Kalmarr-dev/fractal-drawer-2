#include "BasicRecursiveRenderer.cpp"

#include "../../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../../LongDouble/LongDoubleBitset/LongDoubleBitset.h"
#include "../../LongDouble/LongDoubleUInt64/LongDoubleUInt64.h"

template class BasicRecursiveRenderer<BasicLongDouble>;
template class BasicRecursiveRenderer<_LongDoubleBitset>;
template class BasicRecursiveRenderer<_LongDoubleUInt64>;
