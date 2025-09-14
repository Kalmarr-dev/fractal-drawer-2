#include "Stamp.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class Stamp<BasicLongDouble>;
template class Stamp<_LongDoubleBitset>;
template class Stamp<_LongDoubleUInt64>;
