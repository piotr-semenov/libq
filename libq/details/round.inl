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
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
libq::fixed_point<T, n, f, e, op, up>
    round(libq::fixed_point<T, n, f, e, op, up> const& _x)
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;

    typename Q::storage_type const extra = Q(0.5f).value();
    typename Q::storage_type const val = _x.value() + (std::signbit(_x) ? -extra : +extra);
    return Q::wrap(val & Q::integer_bits_mask);
}
} // std

#endif // INC_STD_ROUND_INL_
