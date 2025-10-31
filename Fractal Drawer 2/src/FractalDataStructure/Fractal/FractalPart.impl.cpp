#include "FractalPart.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class FractalPart<BasicLongDouble>;
template class FractalPart<_LongDoubleBitset>;
template class FractalPart<_LongDoubleUInt64>;
