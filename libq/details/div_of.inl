// div_of.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
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
class div_of
    : public type_promotion_base<T, 0, 0, 0> {
    static_assert(std::is_same<T, U>::value,
                  "unexpected use of division_promoted_type_of template");
};

/*!
 \note User has no need to take care about "fixed-point" promotion.
 Presently, std::common_type<L, R>::type equals to type having the max number
 of significant bits.
 \note Consider two numbers \f$x\f$ of format \f$(n_x, f_x, e_x)\f$ and
 \f$y\f$ of format \f$(n_y, f_y, e_y)\f$.
 Let me compute the dynamic range of the quotient \f$z=\frac{x}{y}\f$ of format
 \f$(n_z, f_z, e_z)\f$:
 - the maximum value is \f$\frac{2^{n_x} \cdot 2^{-e_x}}{2^{-f_y} \cdot
   2^{-e_y}} = 2^{n_x+f_y} \cdot 2^{-(e_x-e_y)} = 2^{n_z} \cdot 2^{-e_z}\f$.
 - the minimum value is \f$\frac{2^{-f_x} \cdot 2^{-e_x}}{2^{n_y} \cdot
   2^{-e_y}} = 2^{-(f_x+n_y)} \cdot 2^{-(e_x-e_y)}  = 2^{-f_z} \cdot
   2^{-e_z}\f$.
 So the quotient has the following fixed-point format \f$(n_x+f_y, f_x+n_y,
 e_x-e_y)\f$.
*/
template<typename T_x,
         typename T_y,
         std::size_t n_x,
         std::size_t n_y,
         std::size_t f_x,
         std::size_t f_y,
         int e_x,
         int e_y,
         class op,
         class up>
class div_of<libq::fixed_point<T_x, n_x, f_x, e_x, op, up>,
             libq::fixed_point<T_y, n_y, f_y, e_y, op, up> >
    : public type_promotion_base<libq::fixed_point<T_x, n_x, f_x, e_x, op, up>,
                                 f_y,
                                 n_y,
                                 -e_y> {
};
}  // namespace details
}  // namespace libq

#endif  // INC_LIBQ_DETAILS_DIV_OF_INL_
