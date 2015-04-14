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

#include "boost/mpl/eval_if.hpp"
#include "boost/utility/enable_if.hpp"

namespace libq {
namespace details {

/*!
 \brief
 \tparam T fixed-point type
 \tparam delta_n
 \tparam delta_f
*/
template<typename T, std::size_t delta_n, std::size_t delta_f>
class type_promotion_base
{
public:
    typedef T promoted_type;
};

template<typename _T, std::size_t _n, std::size_t _f, class _op, class _up, std::size_t delta_n, std::size_t delta_f>
class type_promotion_base<libq::fixed_point<_T, _n, _f, _op, _up>, delta_n, delta_f>
{
    typedef libq::fixed_point<_T, _n, _f, _op, _up> Q;
    typedef type_promotion_base<Q, delta_n, delta_f> this_class;

    typedef typename std::conditional<Q::is_signed, std::intmax_t, std::uintmax_t>::type max_type;
    enum: std::size_t {
        sign_bit = static_cast<std::size_t>(Q::is_signed),

        n = Q::number_of_significant_bits,
        f = Q::exponent
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
        is_expandable = (n + delta_n + this_class::sign_bit <= std::numeric_limits<max_type>::digits),
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
        , libq::fixed_point<promoted_storage_type, n + delta_n, f + delta_f, typename Q::overflow_policy, typename Q::underflow_policy>
        , libq::fixed_point<promoted_storage_type, n, f, typename Q::overflow_policy, typename Q::underflow_policy>
    >::type promoted_type;
};

} // details
} // libq
#endif // INC_LIBQ_TYPE_PROMOTION_HPP_
