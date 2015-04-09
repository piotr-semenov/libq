// arctanh_lut.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file arctanh_lut.inl

 Implements the look-up table for trigonometric functions
 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_LIBQ_CORDIC_ARCTANH_LUT_INL_
#define INC_LIBQ_CORDIC_ARCTANH_LUT_INL_

namespace libq {
namespace cordic {

/*!
 \ref page 5, equation 7, m = -1 (hyperbolic coordinate system)
*/
template<size_t n, typename Q>
lut<n, Q>
    lut<n, Q>::hyperbolic_wo_repeated_iterations()
{
    base_class table;

    // Shift sequence is 1, 2, 3, ..., i, ... united with (3k + 1, k = 1, 2, ...).
    // But LUT uses only 1, 2, 3 ,... sequence.
    // see page 10, table 24.1, m = -1
    for (std::size_t i = 0; i != n; ++i) {
        double const arg = 1.0 / std::pow(2.0, double(i + 1u));

        table[i] = Q(-0.5 * std::log(1 - arg) + 0.5 * std::log(1 + arg));
    }

    return this_class(table);
}
} // cordic
} // libq

#endif // INC_LIBQ_CORDIC_ARCTANH_LUT_INL_
