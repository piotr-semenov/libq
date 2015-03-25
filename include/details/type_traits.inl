// type_traits.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file type_traits.inl

 Gets the templates from <type_traits> specialized for fixed-point number type.
*/

#ifndef INC_STD_TYPE_TRAITS_INL_
#define INC_STD_TYPE_TRAITS_INL_

namespace std {
template<typename T, std::size_t n, std::size_t f, class op, class up>
struct is_integral<libq::fixed_point<T, n, f, op, up> >
    : public std::integral_constant<bool, true>
{};

template<typename T, std::size_t n, std::size_t f, class op, class up>
struct make_signed<libq::fixed_point<T, n, f, op, up> >
{
    typedef libq::fixed_point <
        typename std::make_signed<T>::type
        , n
        , f
        , op
        , up > type;
};

template<typename T, std::size_t n, std::size_t f, class op, class up>
struct make_unsigned<libq::fixed_point<T, n, f, op, up> >
{
    typedef libq::fixed_point <
        typename std::make_unsigned<T>::type
        , n
        , f
        , op
        , up > type;
};
} // std

#endif // INC_STD_TYPE_TRAITS_INL_
