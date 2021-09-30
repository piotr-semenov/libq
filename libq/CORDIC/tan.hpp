/** @file tan.hpp
    @brief Provides CORDIC for tan function
    @note see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_CORDIC_TAN_HPP_
#define INC_LIBQ_CORDIC_TAN_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace libq {
namespace details {

template <typename T>
class tan_of
{
public:
    using promoted_type = T;
};

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
class tan_of<libq::fixed_point<T, n, f, e, op, up> >
    : public libq::details::div_of<
          typename libq::details::sin_of<
              libq::fixed_point<T, n, f, e, op, up> >::promoted_type,
          typename libq::details::cos_of<
              libq::fixed_point<T, n, f, e, op, up> >::promoted_type>
{};

}  // namespace details
}  // namespace libq

namespace std {

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    tan(libq::fixed_point<T, n, f, e, op, up> _val) ->
    typename libq::details::tan_of<
        libq::fixed_point<T, n, f, e, op, up> >::promoted_type
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using tan_type = typename libq::details::tan_of<Q>::promoted_type;

    auto const x = std::sin(_val);
    auto const y = std::cos(_val);

    if (0 == y) {
        throw std::logic_error("[std::tan] argument is equal to 0");
    }

    return tan_type(x / y);
}

}  // namespace std

#endif  // INC_LIBQ_CORDIC_TAN_HPP_
