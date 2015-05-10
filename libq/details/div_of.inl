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
    public type_promotion_base<T, 0, 0, 0>
{
    static_assert(std::is_same<T, U>::value, "unexpected use of division_promoted_type_of template");
};

/*!
 \note user has no need to take care about "fixed-point" promotion.
 Presently, std::common_type<L, R>::type equals to type having the max number of significant bits
 \note consider two numbers \f$x\f$ of format \f$(n_x, f_x)\f$ and \f$y\f$ of format \f$(n_y, f_y)\f$.
 Let me compute the dynamic range of the quotient \f$z=\frac{x}{y}\f$ of format \f$(n_z, f_z)\f$
 - the minimum value is \f$\frac{2^{n_x-f_x}}{2^{-f_y}} = 2^{n_x-f_x+f_y} = 2^{n_z-f_z}\f$
 - the maximum value is \f$\frac{2^{-f_x}}{2^{n_y-f_y}} = 2^{-(f_x+(n_y-f_y))} = 2^{-f_z}\f$
 So the quotient has the following fixed-point format \f$(n_x+n_y, f_x+(n_y-f_y))\f$
*/
template<typename T1, typename T2, std::size_t n1, std::size_t n2, std::size_t f1, std::size_t f2, int e1, int e2, class op, class up>
class div_of<libq::fixed_point<T1, n1, f1, e1, op, up>, libq::fixed_point<T2, n2, f2, e2, op, up> >:
    public
        type_promotion_base<libq::fixed_point<T1, n1, f1, e1, op, up>, f2, n2, -e2>
{};

} // details
} // libq

#endif // INC_LIBQ_DETAILS_DIV_OF_INL_
