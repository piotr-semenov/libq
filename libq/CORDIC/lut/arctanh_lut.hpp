/** @file arctanh_lut.hpp
    @brief Implements the look-up table for trigonometric functions.
    @note See H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures".

    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_CORDIC_LUT_ARCTANH_LUT_HPP_
#define INC_LIBQ_CORDIC_LUT_ARCTANH_LUT_HPP_

#include "libq/CORDIC/lut/lut.hpp"

namespace libq {
namespace cordic {

/**
    @note See page 5, equation 7, m = -1 (hyperbolic coordinate system).
*/
template <size_t n, typename Q>
auto
    lut<n, Q>::hyperbolic_wo_repeated_iterations() -> This_class
{
    Base_class table;

    // Shift sequence is 1, 2, 3, ..., i, ... united with (3k + 1, k = 1,2,...).
    // But LUT uses only 1, 2, 3 ,... sequence.
    // see page 10, table 24.1, m = -1
    for (std::size_t i = 0; i != n; ++i) {
        double const arg = 1.0 / std::pow(2.0, static_cast<double>(i + 1u));
        table[i] = Q(-0.5 * std::log(1 - arg) + 0.5 * std::log(1 + arg));
    }

    return This_class(table);
}

}  // namespace cordic
}  // namespace libq

#endif  // INC_LIBQ_CORDIC_LUT_ARCTANH_LUT_HPP_
