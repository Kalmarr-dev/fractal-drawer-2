#include "BasicDataStructure2D.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class BasicDataStructure2D<BasicLongDouble>;
template class BasicDataStructure2D<_LongDoubleBitset>;
template class BasicDataStructure2D<_LongDoubleUInt64>;
