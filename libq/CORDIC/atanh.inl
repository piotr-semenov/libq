// atanh.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file atanh.inl

 Provides CORDIC for atanh function
*/

namespace libq {
namespace details {
/*!
*/
template<typename T>
class atanh_of
{};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class atanh_of<libq::fixed_point<T, n, f, e, op, up> >
    :    public libq::details::sum_traits<
            typename libq::details::log_of<T, n, f, e, op, up>::promoted_type
         >
{};
} // details
} // libq

namespace std {
/*!
 \brief calculates the atanh function as a logarithm
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::atanh_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type
    atanh(libq::fixed_point<T, n, f, e, op, up> _val)
{
    typedef libq::fixed_point<T, n, f, e, op, up> Q;
    typedef typename libq::details::atanh_of<Q>::promoted_type result_type;

    assert(("[std::atanh] argument is not from [-1.0, 1.0]", std::fabs(_val) <= Q(1.0f)));

    result_type x(std::log(_val + 1u) - std::log(Q(1) - _val));
    libq::lift(x) >>= 1u;

    return x;
}
} // std
