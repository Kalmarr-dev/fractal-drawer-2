#include "InputSubject.cpp"

#include "../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../LongDouble/LongDoubleBitset/LongDoubleBitset.h"
#include "../LongDouble/LongDoubleUInt64/LongDoubleUInt64.h"

template class InputSubject<BasicLongDouble>;
template class InputSubject<_LongDoubleBitset>;
template class InputSubject<_LongDoubleUInt64>;
