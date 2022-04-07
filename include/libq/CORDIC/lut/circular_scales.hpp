/** @file circular_scales.hpp
    @brief Computes the scales for CORDIC-rotations performed in circular
    coordinates.
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/

#ifndef INC_LIBQ_CORDIC_LUT_CIRCULAR_SCALES_HPP_
#define INC_LIBQ_CORDIC_LUT_CIRCULAR_SCALES_HPP_

#include "libq/CORDIC/lut/lut.hpp"

namespace libq {
namespace cordic {

template <std::size_t n, typename Q>
double
    lut<n, Q>::circular_scale(std::size_t _n)
{
    double scale(1.0);

    for (std::size_t i = 0; i != _n; ++i) {
        scale *= std::sqrt(1.0 + std::pow(2.0, -2.0 * i));
    }

    return scale;
}

template <std::size_t n, typename Q>
auto
    lut<n, Q>::circular_scales() -> This_class
{
    Base_class scales;

    for (std::size_t i = 0; i != n; ++i) {
        scales[i] = std::sqrt(1.0 + std::pow(2.0, -2.0 * i));
    }

    return This_class(scales);
}

}  // namespace cordic
}  // namespace libq

#endif  // INC_LIBQ_CORDIC_LUT_CIRCULAR_SCALES_HPP_
