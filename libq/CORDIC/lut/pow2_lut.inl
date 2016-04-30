// pow2_lut.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file pow2_lut.inl

 Implements the look-up table for log2 function

 \ref see C. Baumann, "A simple and fast look-up table method to compute the
 exp(x) and ln(x) functions", 2004
*/

#ifndef INC_LIBQ_CORDIC_POW2_LUT_INL_
#define INC_LIBQ_CORDIC_POW2_LUT_INL_

namespace libq {
namespace cordic {

/*!
*/
template<std::size_t n, typename Q>
lut<n, Q>
    lut<n, Q>::pow2()
{
    base_class table;

    for (int i = 1; i != n + 1; ++i) {
        table[i-1] = Q(std::pow(2.0, std::pow(2.0, -i)));
    }

    return this_class(table);
}
} // cordic
} // libq

#endif // INC_LIBQ_CORDIC_POW2_LUT_INL_
