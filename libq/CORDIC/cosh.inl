// cosh.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file cosh.inl

 Provides CORDIC for cosh function as a sum of exponents

 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures" and
 J.S. Walther, "A Unified Algorithm for Elementary Functions"
*/

#ifndef INC_LIBQ_DETAILS_COSH_INL_
#define INC_LIBQ_DETAILS_COSH_INL_

namespace std {
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::sinh_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type  // NOLINT
    cosh(libq::fixed_point<T, n, f, e, op, up> _val) {
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using cosh_type = typename libq::details::sinh_of<Q>::promoted_type;

    auto x = static_cast<cosh_type>(std::exp(_val)) +
        static_cast<cosh_type>(std::exp(-_val));
    libq::lift(x) >>= 1u;

    return x;
}
}  // namespace std

#endif  // INC_LIBQ_DETAILS_COSH_INL_
