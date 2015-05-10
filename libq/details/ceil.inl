// ceil.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file ceil.inl

 Gets the function std::ceil function overloaded for fixed-point numbers.
*/

#ifndef INC_STD_CEIL_INL_
#define INC_STD_CEIL_INL_

namespace std {

/*!
 \brief std::ceil in case of fixed-point numbers
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
libq::fixed_point<T, n, f, e, op, up>
    ceil(libq::fixed_point<T, n, f, e, op, up> const& _x)
{
    typedef libq::fixed_point<T, n, f, e, op, up> Q;

    typename Q::storage_type val = _x.value();
    if (val & Q::fractional_bits_mask) {
        val += Q::storage_type(1u) << Q::bits_for_fractional;
    }

    return Q::make_fixed_point(val & Q::integer_bits_mask);
}
} // std

#endif // INC_STD_CEIL_INL_

