/** @file acosh.hpp
    @brief Provides CORDIC for tanh function as a ratio of sinh and tanh
    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_CORDOC_ACOSH_HPP_
#define INC_LIBQ_CORDOC_ACOSH_HPP_

#include "libq/details/fixed_point_common.hpp"

#include <cassert>

namespace libq {
namespace details {

template <typename T>
class acosh_of
{};

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
class acosh_of<libq::fixed_point<T, n, f, e, op, up> >
{
public:
    using promoted_type =
        typename log_of<T, n, f, e, op, up>::promoted_type::to_unsigned_type;
};

}  // namespace details
}  // namespace libq

namespace std {

/// @brief computes acosh as logarithm
template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    acosh(libq::fixed_point<T, n, f, e, op, up> _val) ->
    typename libq::details::acosh_of<
        libq::fixed_point<T, n, f, e, op, up> >::promoted_type
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using result_type = typename libq::details::acosh_of<Q>::promoted_type;

    assert(("[std::acosh] argument is not from [1.0, +inf)", _val > Q(1.0f)));

    if (_val < Q(1.0f)) {
        throw std::logic_error("[std::acosh] argument is not from [1.0, +inf)");
    }

    result_type const result(std::log(_val + std::sqrt(_val * _val - 1)));
    return result;
}

}  // namespace std

#endif  // INC_LIBQ_CORDOC_ACOSH_HPP_
