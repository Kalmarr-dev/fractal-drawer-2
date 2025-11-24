#include "PSODataProvider.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class PSODataProvider<BasicLongDouble>;
template class PSODataProvider<_LongDoubleBitset>;
template class PSODataProvider<_LongDoubleUInt64>;
