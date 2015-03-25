// fabs.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file fabs.inl

 Gets the function std::fabs overloaded for fixed-point numbers.
*/

#ifndef INC_STD_FABS_INL_
#define INC_STD_FABS_INL_

namespace std {

/*!
 \brief std::fabs in case of fixed-point numbers
*/
template<typename T, std::size_t n, std::size_t f, class op, class up>
libq::fixed_point<T, n, f, op, up>
    fabs(libq::fixed_point<T, n, f, op, up> const& _x)
{
    typedef libq::fixed_point<T, n, f, op, up> Q;

    return (std::signbit(_x)) ? -_x : _x;
}
} // std

#endif // INC_STD_FABS_INL_
