/** @file fmod.hpp
    @brief Gets the function std::round overloaded for fixed-point numbers.
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/

#ifndef INC_LIBQ_DETAILS_ROUND_HPP_
#define INC_LIBQ_DETAILS_ROUND_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace std {

/// @brief std::round in case of fixed-point number.
template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    round(libq::fixed_point<T, n, f, e, op, up> const &_x)
        -> libq::fixed_point<T, n, f, e, op, up>
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;

    typename Q::storage_type const extra = Q(0.5f).value();
    typename Q::storage_type const val =
        _x.value() + (std::signbit(_x) ? -extra : +extra);

    return Q::wrap(val & Q::integer_bits_mask);
}
}  // namespace std

#endif  // INC_LIBQ_DETAILS_ROUND_HPP_
