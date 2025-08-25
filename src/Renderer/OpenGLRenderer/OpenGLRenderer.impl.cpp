#include "OpenGLRenderer.cpp"

#include "../../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../../LongDouble/LongDoubleBitset/LongDoubleBitset.h"

template class OpenGLRenderer<BasicLongDouble>;
template class OpenGLRenderer<_LongDoubleBitset>;
