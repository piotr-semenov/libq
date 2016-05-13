// arctan_lut.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*
 \file arctan_lut.inl

 Implements look-up table for trigonometric functions.
 \ref See H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_LIBQ_CORDIC_ARCTAN_LUT_INL_
#define INC_LIBQ_CORDIC_ARCTAN_LUT_INL_

namespace libq {
namespace cordic {

/*!
 \ref See page 5, equation 7, m = 1 (circular coordinate system).
*/
template<std::size_t n, typename Q>
lut<n, Q> lut<n, Q>::circular() {
    base_class table;

    // shift sequence is just 0, 1, 2, 3, ..., i, ...
    // see page 10, table 24.1, m = 1
    for (std::size_t i = 0; i != n; ++i) {
        double const val = std::atan(1.0 / std::pow(2.0,
                                                    static_cast<double>(i)));

        table[i] = Q(val);
    }

    return this_class(table);
}
}  // namespace cordic
}  // namespace libq

#endif  // INC_LIBQ_CORDIC_ARCTAN_LUT_INL_
