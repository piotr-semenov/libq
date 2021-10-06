/** @file sinh.hpp
    @brief Provides CORDIC for sinh function as a sum of exponents
    @note see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures" and
    J.S. Walther, "A Unified Algorithm for Elementary Functions"

    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_CORDIC_SINH_HPP_
#define INC_LIBQ_CORDIC_SINH_HPP_

#include "libq/details/fixed_point_common.hpp"

#include <limits>

namespace libq {
namespace details {

template <typename T>
class sinh_of
{
    using promoted_type = T;
};

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
class sinh_of<libq::fixed_point<T, n, f, e, op, up> >
{
public:
    using promoted_type =
        libq::fixed_point<std::intmax_t,
                          std::numeric_limits<std::intmax_t>::digits - f,
                          f,
                          e,
                          op,
                          up>;
};

}  // namespace details
}  // namespace libq

namespace std {

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    sinh(libq::fixed_point<T, n, f, e, op, up> _val) ->
    typename libq::details::sinh_of<
        libq::fixed_point<T, n, f, e, op, up> >::promoted_type
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using sinh_type = typename libq::details::sinh_of<Q>::promoted_type;

    auto x = static_cast<sinh_type>(std::exp(_val)) -
             static_cast<sinh_type>(std::exp(-_val));
    libq::lift(x) >>= 1u;

    return x;
}

}  // namespace std

#endif  // INC_LIBQ_CORDIC_SINH_HPP_
