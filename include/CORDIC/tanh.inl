// tanh.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file tanh.inl

 Provides CORDIC for tanh function as a ratio of sinh and tanh
 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_LIBQ_DETAILS_TANH_INL_
#define INC_LIBQ_DETAILS_TANH_INL_

namespace libq {
namespace details {
/*!
*/
template<typename T>
class tanh_of
{
public:
    typedef T promoted_type;
};

template<typename T, std::size_t n, std::size_t f, class op, class up>
class tanh_of<libq::fixed_point<T, n, f, op, up> >
    :   private libq::fixed_point<T, f, f, op, up>,
        public type_promotion_base<
            libq::fixed_point<typename std::make_signed<T>::type, f, f, op, up>,
            1u,
            0
        >
{};
} // details
} // libq

namespace std {
template<typename T, std::size_t n, std::size_t f, class op, class up>
typename libq::details::tanh_of<libq::fixed_point<T, n, f, op, up> >::promoted_type
    tanh(libq::fixed_point<T, n, f, op, up> _val)
{
    typedef libq::fixed_point<T, n, f, op, up> Q;
    typedef typename libq::details::tanh_of<Q>::promoted_type tanh_type;

    auto const a = std::sinh(_val);
    auto const b = std::cosh(_val);
    return tanh_type(std::sinh(_val) / std::cosh(_val));
}
} // std

#endif // INC_LIBQ_DETAILS_TANH_INL_
