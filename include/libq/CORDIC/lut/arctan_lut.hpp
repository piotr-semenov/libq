/** @file arctan_lut.hpp
    @brief Implements look-up table for trigonometric functions.
    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)

    @note See H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/
#ifndef INC_LIBQ_CORDIC_LUT_ARCTAN_LUT_HPP_
#define INC_LIBQ_CORDIC_LUT_ARCTAN_LUT_HPP_

#include "libq/CORDIC/lut/lut.hpp"

namespace libq {
namespace cordic {

/**
    @note See page 5, equation 7, m = 1 (circular coordinate system).
*/
template <std::size_t n, typename Q>
auto
    lut<n, Q>::circular() -> This_class
{
    Base_class table;

    // shift sequence is just 0, 1, 2, 3, ..., i, ...
    // see page 10, table 24.1, m = 1
    for (std::size_t i = 0; i != n; ++i) {
        auto const val = std::atan(1.0 / std::pow(2.0, static_cast<double>(i)));
        table[i] = Q{val};
    }

    return This_class(table);
}

}  // namespace cordic
}  // namespace libq

#endif  // INC_LIBQ_CORDIC_LUT_ARCTAN_LUT_HPP_
