#include "StampPaintDataStructure.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class StampPaintDataStructure<BasicLongDouble>;
template class StampPaintDataStructure<_LongDoubleBitset>;
template class StampPaintDataStructure<_LongDoubleUInt64>;
