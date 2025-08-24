// #include "LongDoubleVector.h"

// #include <math.h>

// LongDoubleVector::LongDoubleVector(double value) {
//   double mantissa = std::frexp(value, &this->current_exponent);
//   int leftover_exponent = this->current_exponent - ((this->current_exponent / this->precision_per_int) * this->precision_per_int);
//   if (leftover_exponent > 0)
//   {
//     mantissa *= (1 << leftover_exponent);
//   } else {
//     mantissa /= (1 << -leftover_exponent);
//   }
//   this->current_exponent -= leftover_exponent;
//   double processed_mantissa = 0;
//   for (int i = 0; i * precision_per_int <= 64; i++)
//   {
//     this->values.push_back((int)((mantissa - processed_mantissa) * (1ll << (i * precision_per_int))));
//     processed_mantissa += ((int)((mantissa - processed_mantissa) * (1ll << (i * precision_per_int)))) * std::pow(2.0, -(i * precision_per_int));
//   }
// }

// int LongDoubleVector::get_sign() const {
//   for (auto &&i : this->values)
//   {
//     if (i < 0)
//     {
//       return -1;
//     }
//     if (i > 0)
//     {
//       return 1;
//     }
//   }
//   return 0;
// }

// // double LongDoubleVector::get_double(ILongDouble* offset, int scale_exponent) {
// //   LongDoubleVector* offset_cast = dynamic_cast<LongDoubleVector*>(offset);
// //   LongDoubleVector ldv = (*this - *offset_cast);
// //   double power_of_2 = 1ll << (ldv.current_exponent - scale_exponent);
// //   if (ldv.current_exponent - scale_exponent < 0)
// //   {
// //     power_of_2 = 1.0 / (1ll << (-ldv.current_exponent + scale_exponent));
// //   }
// //   double d = 0;
// //   for (int i = 0; i * precision_per_int <= 64; i++)
// //   {
// //     d += power_of_2 / (1ll << (i * precision_per_int)) * ldv.values[i];
// //   }
// //   return d;
// // }

// // LongDoubleVector LongDoubleVector::get_normalized() const {
// //   // std::vector<int> new_values;
// //   // int sign = 0;
  
// //   // for (int i = this->values.size() - 1; i > -1; i--)
// //   // {
// //   //   int element = this->values[i];
// //   //   if (std::abs(element) > (1ll << this->precision_per_int))
// //   //   {
// //   //     // TODO
// //   //   }
// //   // }
  
// //   // int first_non_zero_i = -1;
// //   // for (int i = 0; i < this->values.size(); i++)
// //   // {
// //   //   int element = this->values[i];
// //   //   if (first_non_zero_i == -1 && element != 0)
// //   //   {
// //   //     first_non_zero_i = i;
// //   //   }    
// //   // }
// //   // if (first_non_zero_i == -1) first_non_zero_i = this->current_exponent / this->precision_per_int;
// //   // // TODO truncate new_values using first_non_zero_i
// //   // return LongDoubleVector(new_values, this->current_exponent - this->precision_per_int * first_non_zero_i);
// // }
  
// bool operator<(const LongDoubleVector& lhs, const LongDoubleVector& rhs) {
//   if (lhs.get_sign() != rhs.get_sign())
//   {
//     return lhs.get_sign() < rhs.get_sign();
//   }
//   int sign = lhs.get_sign();
//   if (sign == 0)
//   {
//     return false;
//   }
  
//   if (lhs.current_exponent != rhs.current_exponent)
//   {
//     if (sign > 0)
//     {
//       return lhs.current_exponent < rhs.current_exponent;
//     } else if (sign < 0)
//     {
//       return lhs.current_exponent > rhs.current_exponent;
//     }
//   }

//   for (size_t i = 0; i < lhs.values.size(); i++)
//   {
//     if (rhs.values.size() < i)
//     {
//       return lhs.values[i] < 0;
//     } else 
//     {
//       if (lhs.values[i] != rhs.values[i])
//       {
//         return lhs.values[i] < rhs.values[i];
//       }
//     }
//   }
  
//   if (lhs.values.size() < rhs.values.size())
//   {
//     if (sign > 0)
//     {
//       return 0 < rhs.values[lhs.values.size() + 1];
//     } else {
//       return 0 > rhs.values[lhs.values.size() + 1];
//     }
//   }

//   return false;
// }
// // LongDoubleVector operator+(const LongDoubleVector& lhs, const LongDoubleVector& rhs) {
// //   // std::vector<int> accumulator;
// //   // int exponent = std::max(lhs.current_exponent, rhs.current_exponent);

// //   // for (int e = exponent; e > lhs.current_exponent; e -= lhs.precision_per_int)
// //   // {
// //   //   accumulator.push_back(0);
// //   // }
// //   // for (int i = 0; i < lhs.values.size(); i += lhs.precision_per_int)
// //   // {
// //   //   accumulator.push_back(lhs.values[i]);
// //   // }
  
// //   // int starting_index = exponent - rhs.current_exponent;
// //   // for (int i = 0; i < rhs.values.size(); i++)
// //   // {
// //   //   if (i < accumulator.size())
// //   //   {
// //   //     accumulator[starting_index + i] += rhs.values[i];
// //   //   } else {
// //   //     accumulator.push_back(rhs.values[i]);
// //   //   }
// //   // }

// //   // LongDoubleVector res(accumulator, exponent);

// //   // // TODO normalize (every number in the vector is the same sign & less than pow(2, precision_per_int))
// //   // return res.get_normalized();
// // }
// // LongDoubleVector operator-(const LongDoubleVector& lhs, const LongDoubleVector& rhs) {
// //   // TODO
// // }
// // LongDoubleVector operator*(const LongDoubleVector& lhs, const LongDoubleVector& rhs) {

// // }
// // LongDoubleVector operator/(const LongDoubleVector& lhs, const LongDoubleVector& rhs) {

// // }