// sinh.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file sinh.inl

 Provides CORDIC for sinh function as a sum of exponents

 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures" and
 J.S. Walther, "A Unified Algorithm for Elementary Functions"
*/

#ifndef INC_LIBQ_DETAILS_SINH_INL_
#define INC_LIBQ_DETAILS_SINH_INL_

namespace libq {
namespace details {
/*!
*/
template<typename T>
class sinh_of
{
    typedef T promoted_type;
};

template<typename T, std::size_t n, std::size_t f, class op, class up>
class sinh_of<libq::fixed_point<T, n, f, op, up> >
{
public:
    typedef libq::fixed_point<
        std::intmax_t,
        std::numeric_limits<std::intmax_t>::digits,
        f,
        op,
        up
    > promoted_type;
};
} // details
} // libq

namespace std {
template<typename T, std::size_t n, std::size_t f, class op, class up>
typename libq::details::sinh_of<libq::fixed_point<T, n, f, op, up> >::promoted_type
    sinh(libq::fixed_point<T, n, f, op, up> _val)
{
    typedef libq::fixed_point<T, n, f, op, up> Q;
    typedef typename libq::details::sinh_of<Q>::promoted_type sinh_type;

    auto x = static_cast<sinh_type>(std::exp(_val)) - static_cast<sinh_type>(std::exp(-_val));
    libq::lift(x) >>= 1u;

    return x;
}
} // std

#endif // INC_LIBQ_DETAILS_SINH_INL_
