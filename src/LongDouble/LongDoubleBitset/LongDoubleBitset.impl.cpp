#include "LongDoubleBitset.cpp"

template class LongDoubleBitset<1024>;
template bool operator<(const LongDoubleBitset<1024>& lhs, const LongDoubleBitset<1024>& rhs);
template LongDoubleBitset<1024> operator+(const LongDoubleBitset<1024>& lhs, const LongDoubleBitset<1024>& rhs);
template LongDoubleBitset<1024> operator-(const LongDoubleBitset<1024>& lhs, const LongDoubleBitset<1024>& rhs);
template LongDoubleBitset<1024> operator*(const LongDoubleBitset<1024>& lhs, const LongDoubleBitset<1024>& rhs);
template LongDoubleBitset<1024> operator/(const LongDoubleBitset<1024>& lhs, const LongDoubleBitset<1024>& rhs);
template class LongDoubleBitset<1024 + 2>;
