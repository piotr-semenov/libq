// inv_pow2_lut.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \brief Implements the look-up table for trigonometric functions.
 \ref See C. Baumann, "A simple and fast look-up table method to compute the
 exp(x) and ln(x) functions", 2004.
*/

#ifndef INC_LIBQ_CORDIC_INV_POW2_LUT_INL_
#define INC_LIBQ_CORDIC_INV_POW2_LUT_INL_

namespace libq {
namespace cordic {

/*!
*/
template<std::size_t n, typename Q>
lut<n, Q> lut<n, Q>::inv_pow2() {
    base_class table;

    for (int i = 1; i != n + 1; ++i) {
        table[i-1] = Q(1.0 / std::pow(2.0, 1.0 / std::pow(2.0, i)));
    }

    return this_class(table);
}
}  // namespace cordic
}  // namespace libq

#endif  // INC_LIBQ_CORDIC_INV_POW2_LUT_INL_
