#include "ACODataProvider.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class ACODataProvider<BasicLongDouble>;
template class ACODataProvider<_LongDoubleBitset>;
template class ACODataProvider<_LongDoubleUInt64>;
