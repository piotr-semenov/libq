/** @file type_traits.hpp

    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
    Distributed under the New BSD License. (See accompanying file LICENSE)

    @brief Gets the templates from \<type_traits\> specialized for fixed-point
    number type.
*/
#ifndef INC_LIBQ_DETAILS_TYPE_TRAITS_HPP_
#define INC_LIBQ_DETAILS_TYPE_TRAITS_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace std {
template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
struct is_integral<libq::fixed_point<T, n, f, e, op, up> >
    : public std::integral_constant<bool, true>
{};

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
struct make_signed<libq::fixed_point<T, n, f, e, op, up> >
{
    using type =
        libq::fixed_point<typename std::make_signed<T>::type, n, f, e, op, up>;
};

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
struct make_unsigned<libq::fixed_point<T, n, f, e, op, up> >
{
    using type = libq::
        fixed_point<typename std::make_unsigned<T>::type, n, f, e, op, up>;
};

}  // namespace std

#endif  // INC_LIBQ_DETAILS_TYPE_TRAITS_HPP_
