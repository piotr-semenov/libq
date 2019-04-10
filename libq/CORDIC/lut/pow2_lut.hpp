/** @file pow2_lut.hpp
    @brief Implements the look-up table for log2 function.
    @note See C. Baumann, "A simple and fast look-up table method to compute the exp(x) and ln(x) functions", 2004.

    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/

#ifndef INC_LIBQ_CORDIC_POW2_LUT_HPP_
#define INC_LIBQ_CORDIC_POW2_LUT_HPP_

namespace libq {
namespace cordic {

template<std::size_t n, typename Q>
auto
lut<n, Q>::pow2()->This_class
{
    Base_class table;

    for (int i = 1; i != n + 1; ++i) {
        table[i - 1] = Q(std::pow(2.0, std::pow(2.0, -i)));
    }

    return This_class(table);
}

}  // namespace cordic
}  // namespace libq

#endif  // INC_LIBQ_CORDIC_POW2_LUT_HPP_
