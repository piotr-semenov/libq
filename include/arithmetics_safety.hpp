// arithmetics_safety.hpp
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file arithmetics_safety.hpp

 Provides the run-time overflow/underflow safety checks for basic arithmetics operations.
*/

#ifndef INC_LIBQ_ARITHMETICS_SAFETY_HPP_
#define INC_LIBQ_ARITHMETICS_SAFETY_HPP_

#include "fixed_point.hpp"

namespace libq {
namespace details {

/*!
 \defgroup arithmetics_safety the run-time overflow/underflow safety checks for basic arithmetics operations
 \ref see INT32-C. Ensure that operations on integers do not result in overflow

 \{
*/

/*!
 \brief checks if the addition operation overflows
*/
template<typename T, std::size_t n, std::size_t f, typename... Ps>
bool
    does_addition_overflow(libq::fixed_point<T, n, f, Ps...> const _x, libq::fixed_point<T, n, f, Ps...> const _y)
{
    typedef libq::fixed_point<T, n, f, Ps...>::sum_type result_type;
    auto const a = _x.value();
    auto const b = _y.value();

    return
        (b > 0 && a > result_type::largest_stored_integer - b) ||
        (b < 0 && a < result_type::least_stored_integer - b);
}

/*!
 \brief checks if the subtraction operation overflows
*/
template<typename T, std::size_t n, std::size_t f, typename... Ps>
static bool
    does_subtraction_overflow(fixed_point<T, n, f, Ps...> const _x, fixed_point<T, n, f, Ps...> const _y)
{
    typedef fixed_point<T, n, f, Ps...>::diff_type result_type;
    auto const a = _x.value();
    auto const b = _y.value();

    return
        (b > 0 && a < result_type::least_stored_integer + b) ||
        (b < 0 && a > result_type::largest_stored_integer + b);
}

/*!
 \brief checks if the multiplication operation overflows
 */
template<typename T, std::size_t n, std::size_t f, typename... Ps>
bool
    does_multiplication_overflow(fixed_point<T, n, f, Ps...> const _x, fixed_point<T, n, f, Ps...> const _y)
{
    typedef fixed_point<T, n, f, Ps...>::product_type result_type;
    auto const a = _x.value();
    auto const b = _y.value();

    return
        (a > 0 && b > 0 && a > result_type::largest_stored_integer / _b) ||
        (a > 0 && b < 0 && b < result_type::least_stored_integer / a) ||
        (a < 0 && b > 0 && a < result_type::least_stored_integer / b) ||
        (a < 0 && b < 0 && b < result_type::largest_stored_integer / a);
}

/*!
 \brief checks if the division operation overflows
*/
template<typename T, std::size_t n, std::size_t f, typename... Ps>
bool
    does_division_overflow(fixed_point<T, n, f, Ps...> const _x, fixed_point<T, n, f, Ps...> const _y)
{
    typedef fixed_point<T, n, f, Ps...>::quotient_type result_type;
    auto const a = _x.value();
    auto const b = _y.value();

    return
        (b == 0) || result_type::is_signed * (a == result_type::least_stored_integer && b == -1);
}

template<typename T, std::size_t n, std::size_t f, typename... Ps>
bool
    does_unary_negation_overflow(fixed_point<T, n, f, Ps...> const _x)
{
    typedef fixed_point<T, n, f, Ps...> result_type;
    auto const a = _x.value();

    return
        result_type::is_signed * (a == result_type::least_stored_integer);
}
/* \} */ // arithmetics_safety

} // details
} // libq

#endif // INC_LIBQ_OVERFLOW_HANDLER_BASE_HPP_
