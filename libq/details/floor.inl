// floor.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file floor.inl

 Gets the function std::floor overloaded for fixed-point numbers.
*/

#ifndef INC_STD_FLOOR_INL_
#define INC_STD_FLOOR_INL_

namespace std {

/*!
 \brief std::floor in case of fixed-point numbers
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
libq::fixed_point<T, n, f, e, op, up>
    floor(libq::fixed_point<T, n, f, e, op, up> const& _x)
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;

    return Q::wrap(_x.value() & Q::integer_bits_mask);
}
} // std

#endif // INC_STD_FLOOR_INL_
