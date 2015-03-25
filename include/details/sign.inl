// sign.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
\file remainder.inl

Gets the function std::signbit overloaded for fixed-point numbers.
*/

#ifndef INC_STD_SIGN_INL_
#define INC_STD_SIGN_INL_

namespace std {

/*!
\brief function std::signbit determines if the given fixed-point number is negative
*/
template<typename T, std::size_t n, std::size_t f, class op, class up>
bool
    signbit(libq::fixed_point<T, n, f, op, up> const& _x)
{
    typedef libq::fixed_point<T, n, f, op, up> Q;

    return _x.value() < 0;
}
} // std

#endif // INC_STD_SIGN_INL_
