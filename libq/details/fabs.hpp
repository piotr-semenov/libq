/** @file fabs.hpp

    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
    Distributed under the New BSD License. (See accompanying file LICENSE)

    Gets the function std::fabs overloaded for fixed-point numbers.
*/
#ifndef INC_LIBQ_DETAILS_FABS_HPP_
#define INC_LIBQ_DETAILS_FABS_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace std {

/// @brief std::fabs in case of fixed-point numbers
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
libq::fixed_point<T, n, f, e, op, up>
fabs(libq::fixed_point<T, n, f, e, op, up> const& _x)
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;

    return std::signbit(_x) ? -_x : _x;
}

}  // namespace std

#endif  // INC_LIBQ_DETAILS_FABS_HPP_
