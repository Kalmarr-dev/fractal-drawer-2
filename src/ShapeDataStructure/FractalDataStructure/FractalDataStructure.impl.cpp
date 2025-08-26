#include "FractalDataStructure.cpp"

#include "../../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../../LongDouble/LongDoubleBitset/LongDoubleBitset.h"
#include "../../LongDouble/LongDoubleUInt64/LongDoubleUInt64.h"

template class FractalDataStructure<BasicLongDouble>;
template class FractalDataStructure<_LongDoubleBitset>;
template class FractalDataStructure<_LongDoubleUInt64>;
