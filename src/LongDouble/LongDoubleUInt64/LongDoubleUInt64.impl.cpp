#include "LongDoubleUInt64.cpp"

template class LongDoubleUInt64<8>;
template bool operator<(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
template bool operator==(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
template LongDoubleUInt64<8> operator+(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
template LongDoubleUInt64<8> operator-(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
template LongDoubleUInt64<8> operator*(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);
template LongDoubleUInt64<8> operator/(const LongDoubleUInt64<8>& lhs, const LongDoubleUInt64<8>& rhs);

template class LongDoubleUInt64<2>;
template bool operator<(const LongDoubleUInt64<2>& lhs, const LongDoubleUInt64<2>& rhs);
template bool operator==(const LongDoubleUInt64<2>& lhs, const LongDoubleUInt64<2>& rhs);
template LongDoubleUInt64<2> operator+(const LongDoubleUInt64<2>& lhs, const LongDoubleUInt64<2>& rhs);
template LongDoubleUInt64<2> operator-(const LongDoubleUInt64<2>& lhs, const LongDoubleUInt64<2>& rhs);
template LongDoubleUInt64<2> operator*(const LongDoubleUInt64<2>& lhs, const LongDoubleUInt64<2>& rhs);
template LongDoubleUInt64<2> operator/(const LongDoubleUInt64<2>& lhs, const LongDoubleUInt64<2>& rhs);
