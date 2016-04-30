// sum_traits.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file sum_traits.inl

 \brief 
*/

#ifndef INC_LIBQ_DETAILS_SUM_TRAITS_INL_
#define INC_LIBQ_DETAILS_SUM_TRAITS_INL_

namespace libq {
namespace details {
/*!
*/
template<typename T>
class sum_traits
    : public type_promotion_base<T, 1u, 0, 0>
{};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class sum_traits<libq::fixed_point<T, n, f, e, op, up> >
    : public type_promotion_base<libq::fixed_point<T, n, f, e, op, up>, 1u, 0, 0>
{
public:
    static double error(){ return std::exp2(-f+e); }
};

}  // namespace details
}  // namespace libq

#endif  // INC_LIBQ_DETAILS_SUM_TRAITS_INL_
