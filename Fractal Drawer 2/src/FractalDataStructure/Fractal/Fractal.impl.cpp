#include "Fractal.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class Fractal<BasicLongDouble>;
template class Fractal<_LongDoubleBitset>;
template class Fractal<_LongDoubleUInt64>;
