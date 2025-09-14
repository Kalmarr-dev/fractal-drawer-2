#include "OpenGLRenderer.cpp"

#include "../../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../../LongDouble/LongDoubleBitset/LongDoubleBitset.h"
#include "../../LongDouble/LongDoubleUInt64/LongDoubleUInt64.h"

template class OpenGLRenderer<BasicLongDouble>;
template class OpenGLRenderer<_LongDoubleBitset>;
template class OpenGLRenderer<_LongDoubleUInt64>;
