/** @file ceil.hpp
    @brief Gets the function std::ceil function overloaded for fixed-point
    numbers.
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/

#ifndef INC_LIBQ_DETAILS_CEIL_HPP_
#define INC_LIBQ_DETAILS_CEIL_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace std {

/**
 @brief std::ceil in case of fixed-point numbers
*/
template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    ceil(libq::fixed_point<T, n, f, e, op, up> const &_x)
        -> libq::fixed_point<T, n, f, e, op, up>
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;

    typename Q::storage_type val = _x.value();

    if (val & Q::fractional_bits_mask) {
        val += Q::storage_type(1u) << Q::bits_for_fractional;
    }

    return Q::wrap(val & Q::integer_bits_mask);
}
}  // namespace std

#endif  // INC_LIBQ_DETAILS_CEIL_HPP_
