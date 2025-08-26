#include "ScaleRotationMatrix.cpp"

#include "../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../LongDouble/LongDoubleBitset/LongDoubleBitset.h"
#include "../LongDouble/LongDoubleUInt64/LongDoubleUInt64.h"

template class ScaleRotationMatrix<BasicLongDouble>;
template class ScaleRotationMatrix<_LongDoubleBitset>;
template class ScaleRotationMatrix<_LongDoubleUInt64>;
