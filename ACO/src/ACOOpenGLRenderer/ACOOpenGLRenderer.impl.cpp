#include "ACOOpenGLRenderer.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class ACOOpenGLRenderer<BasicLongDouble>;
template class ACOOpenGLRenderer<_LongDoubleBitset>;
template class ACOOpenGLRenderer<_LongDoubleUInt64>;
