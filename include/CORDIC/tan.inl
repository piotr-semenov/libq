// tan.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file tan.inl

 Provides CORDIC for tan function
 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_STD_TAN_INL_
#define INC_STD_TAN_INL_

namespace libq {
namespace details {
/*!
*/
template<typename T>
class tan_of
{
public:
    typedef T promoted_type;
};

template<typename T, std::size_t n, std::size_t f, class op, class up>
class tan_of<libq::fixed_point<T, n, f, op, up> >
    :   public libq::details::div_of<
            typename libq::details::sin_of<libq::fixed_point<T, n, f, op, up> >::promoted_type,
            typename libq::details::cos_of<libq::fixed_point<T, n, f, op, up> >::promoted_type
        >
{};
} // details
} // libq

namespace std {
template<typename T, std::size_t n, std::size_t f, class op, class up>
typename libq::details::tan_of<libq::fixed_point<T, n, f, op, up> >::promoted_type
    tan(libq::fixed_point<T, n, f, op, up> _val)
{
    typedef libq::fixed_point<T, n, f, op, up> Q;
    typedef typename libq::details::tan_of<Q>::promoted_type tan_type;

    auto const x = std::sin(_val);
    auto const y = std::cos(_val);

    if (!y) {
        throw std::logic_error("[std::tan] argument is equal to 0");
    }

    return tan_type(x/y);
}
} // std

#endif // INC_STD_SIN_INL_
