#include "Figure.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class Figure<BasicLongDouble>;
template class Figure<_LongDoubleBitset>;
template class Figure<_LongDoubleUInt64>;
