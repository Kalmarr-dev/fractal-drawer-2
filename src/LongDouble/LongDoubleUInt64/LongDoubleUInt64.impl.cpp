#include "LongDoubleUInt64.cpp"

template class LongDoubleUInt64<8>;
template bool operator<(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
template bool operator==(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
template LongDoubleUInt64<8> operator+(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
template LongDoubleUInt64<8> operator-(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
template LongDoubleUInt64<8> operator*(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
// template LongDoubleUInt64<8> operator/(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
// template class LongDoubleBitset<1024 + 2>;

// template class LongDoubleBitset<64>;
// template bool operator<(const LongDoubleBitset<64>& lhs, const LongDoubleBitset<64>& rhs);
// template bool operator==(const LongDoubleBitset<64>& lhs, const LongDoubleBitset<64>& rhs);
// template LongDoubleBitset<64> operator+(const LongDoubleBitset<64>& lhs, const LongDoubleBitset<64>& rhs);
// template LongDoubleBitset<64> operator-(const LongDoubleBitset<64>& lhs, const LongDoubleBitset<64>& rhs);
// template LongDoubleBitset<64> operator*(const LongDoubleBitset<64>& lhs, const LongDoubleBitset<64>& rhs);
// template LongDoubleBitset<64> operator/(const LongDoubleBitset<64>& lhs, const LongDoubleBitset<64>& rhs);
// template class LongDoubleBitset<64 + 2>;
