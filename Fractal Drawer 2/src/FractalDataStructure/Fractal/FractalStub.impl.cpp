#include "FractalStub.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class FractalStub<BasicLongDouble>;
template class FractalStub<_LongDoubleBitset>;
template class FractalStub<_LongDoubleUInt64>;
