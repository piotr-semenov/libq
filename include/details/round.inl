// round.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
\file fmod.inl

Gets the function std::round overloaded for fixed-point numbers.
*/

#ifndef INC_STD_ROUND_INL_
#define INC_STD_ROUND_INL_

namespace std {

/*!
 \brief std::round in case of fixed-point number
*/
template<typename T, size_t n, size_t f, class op, class up>
libq::fixed_point<T, n, f, op, up>
    round(libq::fixed_point<T, n, f, op, up> const& _x)
{
    typedef libq::fixed_point<T, n, f, op, up> Q;

    Q::storage_type const extra = Q(0.5f).value();
    Q::storage_type const val = _x.value() + (std::signbit(_x) ? -extra : +extra);
    return Q::make_fixed_point(val & Q::integer_bits_mask);
}
} // std

#endif // INC_STD_ROUND_INL_
