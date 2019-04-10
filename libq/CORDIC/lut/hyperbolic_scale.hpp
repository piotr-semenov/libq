/** @file hyperbolic_scale.hpp
    
    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
    Distributed under the New BSD License. (See accompanying file LICENSE)

    Computes the look-up table CORDIC-rotations performed in the hyperbolic
    coordinates.
*/

#ifndef INC_LIBQ_CORDIC_HYPERBOLIC_SCALE_HPP_
#define INC_LIBQ_CORDIC_HYPERBOLIC_SCALE_HPP_

#include "libq/CORDIC/lut/lut.hpp"

namespace libq {
namespace cordic {

template<std::size_t n, typename Q>
double
lut<n, Q>::hyperbolic_scale_with_repeated_iterations(std::size_t _n)
{
    double scale(1.0);
    std::size_t repeated(4u);

    for (std::size_t i = 1u; i != _n; ++i) {
        scale *= std::sqrt(1.0 - std::pow(2.0, -2.0 * i));

        if (i == repeated && i != _n - 1) {
            scale *= std::sqrt(1.0 - std::pow(2.0, -2.0 * i++));
            repeated = 3u * repeated + 1u;
        }
    }

    return scale;
}

}  // namespace cordic
}  // namespace libq

#endif  // INC_LIBQ_CORDIC_HYPERBOLIC_SCALE_HPP_
