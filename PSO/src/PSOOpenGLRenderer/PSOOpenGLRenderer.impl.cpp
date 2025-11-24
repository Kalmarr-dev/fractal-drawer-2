#include "PSOOpenGLRenderer.cpp"

#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>

template class PSOOpenGLRenderer<BasicLongDouble>;
template class PSOOpenGLRenderer<_LongDoubleBitset>;
template class PSOOpenGLRenderer<_LongDoubleUInt64>;
