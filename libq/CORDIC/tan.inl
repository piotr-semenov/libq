// tan.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file tan.inl

 Provides CORDIC for tan function
 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_STD_TAN_INL_
#define INC_STD_TAN_INL_

namespace libq {
namespace details {
/*!
*/
template<typename T>
class tan_of {
 public:
    using promoted_type = T;
};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class tan_of<libq::fixed_point<T, n, f, e, op, up> >
    : public libq::details::div_of<
        typename libq::details::sin_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type,  // NOLINT
        typename libq::details::cos_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type> {  // NOLINT
};
}  // namespace details
}  // namespace libq


namespace std {
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::tan_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type  // NOLINT
    tan(libq::fixed_point<T, n, f, e, op, up> _val) {
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using tan_type = typename libq::details::tan_of<Q>::promoted_type;

    auto const x = std::sin(_val);
    auto const y = std::cos(_val);

    if (!y) {
        throw std::logic_error("[std::tan] argument is equal to 0");
    }

    return tan_type(x/y);
}
}  // namespace std

#endif  // INC_STD_SIN_INL_
