// remainder.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
\file remainder.inl

Gets the function std::remainder overloaded for fixed-point numbers.
*/

#ifndef INC_STD_REMAINDER_INL_
#define INC_STD_REMAINDER_INL_

namespace std {

/*!
\brief function std::fmod computes fixed-point remainder of double(x)/double(y)
*/
template<typename T1, typename T2, std::size_t n1, std::size_t n2, std::size_t f1, std::size_t f2, class op, class up>
libq::fixed_point<T2, n2, f2, op, up>
    remainder(libq::fixed_point<T1, n1, f1, op, up> const& _x, libq::fixed_point<T2, n2, f2, op, up> const& _y)
{
    typedef libq::fixed_point<T2, n2, f2, op, up> Q;

    auto const _tmp1 = _x / _y;
    auto const _tmp2 = std::round(_x / _y);
    return static_cast<Q>(_x - std::round(_x / _y) * _y);
}
} // std

#endif // INC_STD_REMAINDER_INL_
