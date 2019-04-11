/** @file remainder.hpp
    @brief Gets the function std::signbit overloaded for fixed-point numbers.

    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/

#ifndef INC_LIBQ_DETAILS_SIGN_HPP_
#define INC_LIBQ_DETAILS_SIGN_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace std {

/**
 @brief Function std::signbit determines if the given fixed-point number is
 negative.
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
bool
    signbit(libq::fixed_point<T, n, f, e, op, up> const& _x) {
    using Q = libq::fixed_point<T, n, f, e, op, up>;

    return _x.value() < 0;
}
}  // namespace std

#endif  // INC_LIBQ_DETAILS_SIGN_HPP_
