#include "ScaleRotationMatrix.cpp"

#include "../LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "../LongDouble/LongDoubleBitset/LongDoubleBitset.h"

template class ScaleRotationMatrix<BasicLongDouble>;
template class ScaleRotationMatrix<_LongDoubleBitset>;
