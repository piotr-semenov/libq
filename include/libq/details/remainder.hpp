/** @file remainder.hpp
    @brief Gets the function std::remainder overloaded for fixed-point numbers.
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/

#ifndef INC_LIBQ_DETAILS_REMAINDER_HPP_
#define INC_LIBQ_DETAILS_REMAINDER_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace std {

/**
 @brief Function std::fmod computes fixed-point remainder of
 double(x)/double(y).
*/
template <typename T1,
          typename T2,
          std::size_t n1,
          std::size_t n2,
          std::size_t f1,
          std::size_t f2,
          int         e1,
          int         e2,
          class op,
          class up>
auto
    remainder(libq::fixed_point<T1, n1, f1, e1, op, up> const &_x,
              libq::fixed_point<T2, n2, f2, e2, op, up> const &_y)
        -> libq::fixed_point<T2, n2, f2, e2, op, up>
{
    using Q = libq::fixed_point<T2, n2, f2, e2, op, up>;

    return static_cast<Q>(_x - std::round(_x / _y) * _y);
}
}  // namespace std

#endif  // INC_LIBQ_DETAILS_REMAINDER_HPP_
