/** @file floor.hpp

    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
    Distributed under the New BSD License. (See accompanying file LICENSE)

    Gets the function std::floor overloaded for fixed-point numbers.
*/
#ifndef INC_LIBQ_DETAILS_FLOOR_HPP_
#define INC_LIBQ_DETAILS_FLOOR_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace std {

/// @brief std::floor in case of fixed-point numbers
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
libq::fixed_point<T, n, f, e, op, up>
floor(libq::fixed_point<T, n, f, e, op, up> const& _x)
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;

    return Q::wrap(_x.value() & Q::integer_bits_mask);
}

}  // namespace std

#endif  // INC_LIBQ_DETAILS_FLOOR_HPP_
