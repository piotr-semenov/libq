/** @brief Implements the look-up table for trigonometric functions.
    @note See C. Baumann, "A simple and fast look-up table method to compute the
    exp(x) and ln(x) functions", 2004.

    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/

#ifndef INC_LIBQ_CORDIC_LUT_INV_POW2_LUT_HPP_
#define INC_LIBQ_CORDIC_LUT_INV_POW2_LUT_HPP_

#include "libq/CORDIC/lut/lut.hpp"

namespace libq {
namespace cordic {

template <std::size_t n, typename Q>
auto
    lut<n, Q>::inv_pow2() -> This_class
{
    Base_class table;

    for (std::size_t i = 1; i != n + 1; ++i) {
        table[i - 1] = Q(1.0 / std::pow(2.0, 1.0 / std::pow(2.0, i)));
    }

    return This_class(table);
}

}  // namespace cordic
}  // namespace libq

#endif  // INC_LIBQ_CORDIC_LUT_INV_POW2_LUT_HPP_
