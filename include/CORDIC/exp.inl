// exp.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file exp.inl

 Provides CORDIC for exp function

 \ref see C. Baumann, "A simple and fast look-up table method to compute the
 exp(x) and ln(x) functions", 2004
*/

#ifndef INC_LIBQ_DETAILS_EXP_INL_
#define INC_LIBQ_DETAILS_EXP_INL_

namespace libq {
namespace details {
/*!
*/
template<typename T>
class exp_of
{
public:
    typedef T promoted_type;
};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class exp_of<libq::fixed_point<T, n, f, e, op, up> >
{
public:
    typedef libq::fixed_point<
        std::uintmax_t,
        std::numeric_limits<std::uintmax_t>::digits - f,
        f,
        e,
        op,
        up
    > promoted_type;
};
} // details
} // libq

namespace std {
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::exp_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type
    exp(libq::fixed_point<T, n, f, e, op, up> _val)
{
    typedef libq::fixed_point<T, n, f, e, op, up> Q;
    typedef typename libq::details::exp_of<Q>::promoted_type exp_type;

    typedef libq::Q<f, f, e, op, up> Qw;
    typedef typename libq::details::type_promotion_base<Qw, 1u, 0, 0>::promoted_type
        work_type;
    typedef libq::cordic::lut<f, work_type> lut_type;

    // reduces argument to interval [0.0, 1.0]
    int power(0);
    typename libq::details::mult_of<Q, work_type>::promoted_type arg(_val * work_type::CONST_LOG2E);
    while (arg >= exp_type(1.0)) {
        arg = arg - 1u;
        power++;
    }
    while (arg < exp_type(0.0)) {
        arg = arg + 1u;
        power--;
    }

    static lut_type const pow2_lut = lut_type::pow2();
    exp_type result(1.0);
    work_type x(arg);
    for (std::size_t i = 0; i != f; ++i) {
        work_type const pow2 = work_type::make_fixed_point(T(1u) << (f - i - 1u));

        if (x - pow2 >= work_type(0.0)) {
            x = x - pow2;

            result *= pow2_lut[i];
        }
    }

    if (power >= 0) {
        libq::lift(result) <<= power;
    }
    else {
        libq::lift(result) >>= (-power);
    }
    return result;
}
} // std

#endif // INC_LIBQ_DETAILS_EXP_INL_
