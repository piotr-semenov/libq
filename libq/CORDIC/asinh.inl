// asinh.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file asinh.inl

 Provides CORDIC for tanh function as a ratio of sinh and tanh
*/

#ifndef INC_STD_ASINH_INL_
#define INC_STD_ASINH_INL_


namespace libq {
namespace details {
/*!
 \brief
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class asinh_of
    : public log_of<T, n, f, e, op, up> {
};
}  // namespace details
}  // namespace libq


namespace std {
/*!
 \brief computes arcinh as logarithm
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::asinh_of<T, n, f, e, op, up>::promoted_type
    asinh(libq::fixed_point<T, n, f, e, op, up> _val) {
    using result_type = typename libq::details::asinh_of<T, n, f, e, op, up>::promoted_type;  // NOLINT

    return result_type(
        std::log(std::sqrt(_val * _val + 1u) + _val));
}
}  // namespace std

#endif  // INC_STD_ASINH_INL_
