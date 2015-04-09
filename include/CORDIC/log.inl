// log.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file log.inl

 Provides CORDIC for ln function
 \ref see C. Baumann, "A simple and fast look-up table method to compute the
 exp(x) and ln(x) functions", 2004
*/

#ifndef INC_STD_LOG_INL_
#define INC_STD_LOG_INL_

#include "boost/integer/static_min_max.hpp"
#include "boost/integer/static_log2.hpp"

#include <cassert>

namespace libq {
namespace details {
/*!
 \brief
*/
template<typename T, std::size_t n, std::size_t f, typename op, typename up>
class log_of:
    public type_promotion_base<
        fixed_point<typename std::make_signed<T>::type, n, f, op, up>
        , boost::static_unsigned_max<boost::static_log2<f>::value + 1u, n + boost::static_log2<n>::value + 1u>::value
        , 0
    >
{};
} // details
} // libq

namespace std {
template<typename T, std::size_t n, std::size_t f, typename op, typename up>
typename libq::details::log_of<T, n, f, op, up>::promoted_type
    log(libq::fixed_point<T, n, f, op, up> _val)
{
    typedef libq::fixed_point<T, n, f, op, up> Q;
    typedef typename libq::details::log_of<T, n, f, op, up>::promoted_type log_type;
    typedef libq::cordic::lut<f, Q> lut;

    assert(("[std::log] argument is negaitve", _val >= Q(0)));
    if (_val <= Q(0)) {
        throw std::logic_error("[std::log]: argument is negative");
    }

    // one need 1 bit to represent integer part of reals from [1.0, 2.0]
    typedef libq::fixed_point<typename boost::uint_t<n + 1u>::least, f + 1u, f, op, up> work_type;

    // reduces argument to interval [1.0, 2.0]
    int power(0);
    Q arg(_val);
    while (arg >= Q(2.0)) {
        libq::lift(arg) >>= 1u;
        power++;
    }
    while (arg < Q(1.0)) {
        libq::lift(arg) <<= 1u;
        power--;
    }

    // one can consider 0 < y = log(2, x) < 1 as x = 2^y
    // so CORDIC rotation is just a multiplication by 2^{1/2^i}:
    // 2^y = 2^{a1/2} * 2^{a2/4} * ... * 2^{ai/2^i}, where ai is from
    // {0, 1}
    static libq::cordic::lut<f, Q> const inv_pow2_lut = lut::inv_pow2();

    work_type result(0);
    for (std::size_t i = 0; i != f; ++i) {
        if (work_type(arg * inv_pow2_lut[i]) >= work_type(1.0)) {
            arg = work_type(arg * inv_pow2_lut[i]);

            libq::lift(result) += typename work_type::storage_type(1u) << (f - i - 1u);
        }
    }

    log_type const r0(log_type(result) + log_type(power));
    log_type const r1(r0 * work_type::CONST_1_LOG2E);

    return r1;
}
} // std

#endif // INC_STD_LOG_INL_
