#include "BasicCamera.cpp"

#include "../../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../../LongDouble/LongDoubleBitset/LongDoubleBitset.h"
#include "../../LongDouble/LongDoubleUInt64/LongDoubleUInt64.h"

template class BasicCamera<BasicLongDouble>;
template class BasicCamera<_LongDoubleBitset>;
template class BasicCamera<_LongDoubleUInt64>;
