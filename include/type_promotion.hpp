// type_promotion.hpp
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
\file type_promotion.hpp

Provides the run-time overflow/underflow safety checks for basic arithmetics operations.
*/

#ifndef INC_LIBQ_TYPE_PROMOTION_HPP_
#define INC_LIBQ_TYPE_PROMOTION_HPP_

#include "boost/integer.hpp"
#include "boost/mpl/eval_if.hpp"
#include "boost/utility/enable_if.hpp"

namespace libq {

// forward declaration
template<typename T, std::size_t n, std::size_t f, typename op, typename up>
class fixed_point;

namespace details {

/*!
 \brief
 \tparam Fixed_point_type
 \tparam delta_n
 \tparam delta_f
*/
template<typename Base_fixed_point_type, int delta_n = 0, int delta_f = 0>
class type_promotion_base
{
    typedef Base_fixed_point_type Q;
    typedef type_promotion_base<Q, delta_n, delta_f> this_class;

    static_assert(Q::number_of_significant_bits + delta_n >= 0, "promoted type has a negative number of significant bits");
    static_assert(Q::bits_for_fractional + delta_f >= 0, "promoted type has a negative number of bits for fractional");

    typedef typename std::conditional<Q::is_signed, std::intmax_t, std::uintmax_t>::type max_type;
    enum: std::size_t {
        sign_bit = static_cast<std::size_t>(Q::is_signed),

        n = Q::number_of_significant_bits,
        f = Q::bits_for_fractional
    };

    // simple "type" wrapper for lazy instantiation of its "internal" type
    struct storage_type_promotion_traits
    {
        typedef typename std::conditional<
            Q::is_signed
            , typename boost::int_t<n + delta_n + this_class::sign_bit>::least // boost::int_t takes a sign bit into account
            , typename boost::uint_t<n + delta_n>::least
        >::type type;
    };
    struct storage_type_default_traits
    {
        typedef typename Q::storage_type type;
    };

public:

    enum: bool {
        is_expandable = (n + delta_n + this_class::sign_bit <= std::numeric_limits<max_type>::digits)
    };

    /*!
     \brief
    */
    typedef typename boost::mpl::eval_if_c<
        this_class::is_expandable
        , typename this_class::storage_type_promotion_traits
        , typename this_class::storage_type_default_traits
    >::type promoted_storage_type;

    typedef typename std::conditional<
        this_class::is_expandable
        , libq::fixed_point<promoted_storage_type, n + delta_n, f + delta_f, typename Q::op, typename Q::up>
        , Q
    >::type promoted_type;
};

template<int n_delta, int f_delta>
class type_promotion_base<double, n_delta, f_delta>
{
public:
    typedef double promoted_type;
};

template<int n_delta, int f_delta>
class type_promotion_base<float, n_delta, f_delta>
{
public:
    typedef float promoted_type;
};

/// sum_promoted_type_of
template<typename T>
class sum_of:
    public type_promotion_base<T, 0, 0>
{};

template<typename T, std::size_t n, std::size_t f, typename op, typename up>
class sum_of<libq::fixed_point<T, n, f, op, up> >:
    public type_promotion_base<libq::fixed_point<T, n, f, op, up>, 1u, 0>
{};

/// multiplication_promoted_type_of
template<typename T, typename U>
class mult_of:
    public type_promotion_base<T>
{
    static_assert(std::is_same<T, U>::value, "unexpected use of multiplication_promoted_type_of template");
};

template<typename T1, typename T2, std::size_t n1, std::size_t n2, std::size_t f1, std::size_t f2, typename op, typename up>
class mult_of<libq::fixed_point<T1, n1, f1, op, up>, libq::fixed_point<T2, n2, f2, op, up> > :
    public type_promotion_base<libq::fixed_point<T1, n1, f1, op, up>, n2, f2>
{};

template<typename T, typename U>
class div_of:
    public type_promotion_base<T>
{
    static_assert(std::is_same<T, U>::value, "unexpected use of division_promoted_type_of template");
};

template<typename T1, typename T2, std::size_t n1, std::size_t n2, std::size_t f1, std::size_t f2, typename op, typename up>
class div_of<libq::fixed_point<T1, n1, f1, op, up>, libq::fixed_point<T2, n2, f2, op, up> >:
    public type_promotion_base<libq::fixed_point<T1, n1, f1, op, up>, n2, n2 - f2>
{};

} // details
} // libq
#endif // INC_LIBQ_TYPE_PROMOTION_HPP_
