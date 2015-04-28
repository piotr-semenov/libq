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

#include <stdexcept>

namespace libq {

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class fixed_point;

/*!
*/
class overflow_exception_policy
{
public:
    enum: bool {
        does_throw = true
    };

    static void raise_event(char const* _msg = nullptr)
    {
        throw std::overflow_error(_msg);
    }
};

/*!
*/
class underflow_exception_policy
{
public:
    enum: bool {
        does_throw = true
    };

    static void raise_event(char const* _msg = nullptr)
    {
        throw std::underflow_error(_msg);
    }
};

/*!
*/
class ignorance_policy
{
public:
    enum: bool {
        does_throw = false
    };

    static void raise_event(char const* = nullptr){}
};

namespace details {

template<typename T> class sum_of;
template<typename T1, typename T2> class mult_of;
template<typename T1, typename T2> class div_of;

/*!
 \defgroup arithmetics_safety the run-time overflow/underflow safety checks for basic arithmetics operations
 \ref see INT32-C. Ensure that operations on integers do not result in overflow

 \{
*/

/*!
 \brief checks if the addition operation overflows
*/
template<typename T, std::size_t n, std::size_t f, int e, typename... Ps>
bool
    does_addition_overflow(libq::fixed_point<T, n, f, e, Ps...> const _x, libq::fixed_point<T, n, f, e, Ps...> const _y)
{
    typedef typename sum_of<fixed_point<T, n, f, e, Ps...> >::promoted_type result_type;
    auto const a = _x.value();
    auto const b = _y.value();
    auto const _tmp = result_type::largest_stored_integer;

    return
        (b > 0 && a > result_type::largest_stored_integer - b) ||
        (b < 0 && a < result_type::least_stored_integer - b);
}

/*!
 \brief checks if the subtraction operation overflows
*/
template<typename T, std::size_t n, std::size_t f, int e, typename... Ps>
static bool
    does_subtraction_overflow(fixed_point<T, n, f, e, Ps...> const _x, fixed_point<T, n, f, e, Ps...> const _y)
{
    typedef typename sum_of<fixed_point<T, n, f, e, Ps...> >::promoted_type result_type;
    auto const a = _x.value();
    auto const b = _y.value();

    return
        (b > 0 && a < result_type::least_stored_integer + b) ||
        (b < 0 && a > result_type::largest_stored_integer + b);
}

/*!
 \brief checks if the multiplication operation overflows
 */
template<typename T1, std::size_t n1, std::size_t f1, typename T2, int e1, std::size_t n2, std::size_t f2, int e2, typename... Ps>
bool
    does_multiplication_overflow(libq::fixed_point<T1, n1, f1, e1, Ps...> const _x, libq::fixed_point<T2, n2, f2, e2, Ps...> const _y)
{
    typedef libq::fixed_point<T1, n1, f1, e1, Ps...> Q1;
    typedef libq::fixed_point<T2, n2, f2, e2, Ps...> Q2;
    
    typedef typename mult_of<Q1, Q2>::promoted_type result_type;
    typedef typename result_type::storage_type promoted_storage_type;

    promoted_storage_type const a = static_cast<promoted_storage_type>(_x.value());
    promoted_storage_type const b = static_cast<promoted_storage_type>(_y.value());

    return
        (a > 0 && b > 0 && a > result_type::largest_stored_integer / b) ||
        (a > 0 && b < 0 && b < result_type::least_stored_integer / a) ||
        (a < 0 && b > 0 && a < result_type::least_stored_integer / b) ||
        (a < 0 && b < 0 && b < result_type::largest_stored_integer / a);
}

/*!
 \brief checks if the division operation overflows
*/
template<typename T1, std::size_t n1, std::size_t f1, int e1, typename T2, std::size_t n2, std::size_t f2, int e2, typename... Ps>
bool
    does_division_overflow(libq::fixed_point<T1, n1, f1, e1, Ps...> const _x, libq::fixed_point<T2, n2, f2, e2, Ps...> const _y)
{
    typedef typename div_of<libq::fixed_point<T1, n1, f1, e1, Ps...>, libq::fixed_point<T2, n2, f2, e2, Ps...> >::promoted_type result_type;
    auto const a = _x.value();
    auto const b = _y.value();

    return
        (b == 0) || result_type::is_signed * (a == result_type::least_stored_integer && b == -1);
}

template<typename T, std::size_t n, std::size_t f, int e, typename... Ps>
bool
    does_unary_negation_overflow(fixed_point<T, n, f, e, Ps...> const _x)
{
    typedef fixed_point<T, n, f, e, Ps...> result_type;
    auto const a = _x.value();

    return
        result_type::is_signed && (a == result_type::least_stored_integer);
}
/* \} */ // arithmetics_safety

} // details
} // libq

#endif // INC_LIBQ_OVERFLOW_HANDLER_BASE_HPP_
