// sum_of.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file sum_of.inl

*/

#ifndef INC_LIBQ_DETAILS_SUM_OF_INL_
#define INC_LIBQ_DETAILS_SUM_OF_INL_

namespace libq {
namespace details {
/*!
*/
template<typename T>
class sum_of:
    public type_promotion_base<T, 1u, 0, 0>
{};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class sum_of<libq::fixed_point<T, n, f, e, op, up> > :
    public type_promotion_base<libq::fixed_point<T, n, f, e, op, up>, 1u, 0, 0>
{};

} // details
} // libq

#endif // INC_LIBQ_DETAILS_SUM_OF_INL_
