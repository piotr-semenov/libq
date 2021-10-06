/** @file fmod.hpp
    @brief Gets the function std::fmod overloaded for fixed-point numbers.

    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_DETAILS_FMOD_HPP_
#define INC_LIBQ_DETAILS_FMOD_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace std {

/**
 @brief function std::fmod computes fixed-point remainder of double(x)/double(y)
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
libq::fixed_point<T2, n2, f2, e2, op, up>
    fmod(libq::fixed_point<T1, n1, f1, e1, op, up> const &_x,
         libq::fixed_point<T2, n2, f2, e2, op, up> const &_y)
{
    using Q = libq::fixed_point<T2, n2, f2, e2, op, up>;

    auto const y = std::fabs(_y);
    auto const result = std::remainder(std::fabs(_x), y);

    return std::signbit(result) ? static_cast<Q>(result + y)
                                : static_cast<Q>(result);
}
}  // namespace std

#endif  // INC_LIBQ_DETAILS_FMOD_HPP_
