#include "GLFWInput.cpp"

#include "../../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../../LongDouble/LongDoubleBitset/LongDoubleBitset.h"
#include "../../LongDouble/LongDoubleUInt64/LongDoubleUInt64.h"

template class GLFWInput<BasicLongDouble>;
template class GLFWInput<_LongDoubleBitset>;
template class GLFWInput<_LongDoubleUInt64>;
