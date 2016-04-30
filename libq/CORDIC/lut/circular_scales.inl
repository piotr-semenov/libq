// circular_scales.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file circular_scales.inl

 Computes the scales for CORDIC-rotations performed in circular coordinates
*/

#ifndef INC_LIBQ_CORDIC_CIRCULAR_SCALES_INL_
#define INC_LIBQ_CORDIC_CIRCULAR_SCALES_INL_

namespace libq {
namespace cordic {

/*!
*/
template<std::size_t n, typename Q>
double
    lut<n, Q>::circular_scale(std::size_t _n)
{
    double scale(1.0);

    for (std::size_t i = 0; i != _n; ++i) {
        scale *= std::sqrt(1.0 + std::pow(2.0, -2.0 * i));
    }

    return scale;
}

/*!
*/
template<std::size_t n, typename Q>
lut<n, Q>
    lut<n, Q>::circular_scales()
{
    base_class scales;

    for (std::size_t i = 0; i != n; ++i) {
        scales[i] = std::sqrt(1.0 + std::pow(2.0, -2.0 * i));
    }

    return this_class(scales);
}
} // cordic
} // libq

#endif // INC_LIBQ_CORDIC_CIRCULAR_SCALES_INL_
