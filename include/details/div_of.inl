// div_of.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file div_of.inl

*/

#ifndef INC_LIBQ_DETAILS_DIV_OF_INL_
#define INC_LIBQ_DETAILS_DIV_OF_INL_

namespace libq {
namespace details {
/*!
*/
template<typename T, typename U>
class div_of:
    public type_promotion_base<T, 0, 0>
{
    static_assert(std::is_same<T, U>::value, "unexpected use of division_promoted_type_of template");
};

// /*!
//  \note user has no need to take care about "fixed-point" promotion.
//  Presently, std::common_type<L, R>::type equals to type having the max number of significant bits
// */
template<typename T1, typename T2, std::size_t n1, std::size_t n2, std::size_t f1, std::size_t f2, typename op, typename up>
class div_of<libq::fixed_point<T1, n1, f1, op, up>, libq::fixed_point<T2, n2, f2, op, up> >:
    public
        /*std::conditional<
            (n1 > n2),*/
            type_promotion_base<libq::fixed_point<T1, n1, f1, op, up>, n2, (n2 >= f2) ? (n2 - f2) : 0>/*,
            type_promotion_base<libq::fixed_point<T2, n2, f2, op, up>, n1, (n1 >= f1) ? (n1 - f1) : 0>
        >::type*/
{};

} // details
} // libq

#endif // INC_LIBQ_DETAILS_DIV_OF_INL_
