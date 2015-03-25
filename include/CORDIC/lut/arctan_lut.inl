// arctan_lut.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*
 \file arctan_lut.inl

 Implements look-up table for trigonometric functions
 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_LIBQ_CORDIC_ARCTAN_LUT_INL_
#define INC_LIBQ_CORDIC_ARCTAN_LUT_INL_

namespace libq {
namespace cordic {

/// @ref page 5, equation 7, m = 1 (circular coordinate system)
template<size_t n, typename fp>
typename lut<n, fp> lut<n, fp>::circular()
{
    base_class table;

    // shift sequence is just 0, 1, 2, 3, ..., i, ...
    // see page 10, table 24.1, m = 1
    for (std::size_t i = 0; i != n; ++i) {
        table[i] = Q(std::atan(1.0 / std::pow(2.0, double(i))));
    }

    return this_class(table);
}
} // cordic
} // libq

#endif // INC_LIBQ_CORDIC_ARCTAN_LUT_INL_
