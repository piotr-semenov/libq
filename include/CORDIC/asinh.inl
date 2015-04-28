// asinh.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file asinh.inl

 Provides CORDIC for tanh function as a ratio of sinh and tanh
*/

namespace libq {
namespace details {
/*!
 \brief
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class asinh_of
    :    public log_of<T, n, f, e, op, up>
{};
} // details
} // libq

namespace std {
/*!
 \brief computes arcinh as logarithm
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::asinh_of<T, n, f, e, op, up>::promoted_type
    asinh(libq::fixed_point<T, n, f, e, op, up> _val)
{
    typedef typename libq::details::asinh_of<T, n, f, e, op, up>::promoted_type result_type;

    return result_type(
        std::log(std::sqrt(_val * _val + 1u) + _val)
    );
}
} // std
