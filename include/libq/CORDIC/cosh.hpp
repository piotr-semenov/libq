/** @file cosh.hpp
    @brief Provides CORDIC for cosh function as a sum of exponents
    @note See H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
    and J.S. Walther, "A Unified Algorithm for Elementary Functions"

    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_CORDIC_COSH_HPP_
#define INC_LIBQ_CORDIC_COSH_HPP_

#include "libq/CORDIC/sinh.hpp"

namespace std {

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    cosh(libq::fixed_point<T, n, f, e, op, up> _val) ->
    typename libq::details::sinh_of<
        libq::fixed_point<T, n, f, e, op, up> >::promoted_type
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using cosh_type = typename libq::details::sinh_of<Q>::promoted_type;

    auto x = static_cast<cosh_type>(std::exp(_val)) +
             static_cast<cosh_type>(std::exp(-_val));
    libq::lift(x) >>= 1u;

    return x;
}

}  // namespace std

#endif  // INC_LIBQ_CORDIC_COSH_HPP_
