
/** @file asinh.hpp
    @brief Provides CORDIC for tanh function as a ratio of sinh and tanh
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_CORDIC_ASINH_HPP_
#define INC_LIBQ_CORDIC_ASINH_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace libq {
namespace details {
template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
class asinh_of : public log_of<T, n, f, e, op, up>
{};

}  // namespace details
}  // namespace libq

namespace std {

/// @brief computes arcsinh as logarithm
template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    asinh(libq::fixed_point<T, n, f, e, op, up> _val) ->
    typename libq::details::asinh_of<T, n, f, e, op, up>::promoted_type
{
    using result_type =
        typename libq::details::asinh_of<T, n, f, e, op, up>::promoted_type;

    return result_type(std::log(std::sqrt(_val * _val + 1u) + _val));
}

}  // namespace std

#endif  // INC_LIBQ_CORDIC_ASINH_HPP_
