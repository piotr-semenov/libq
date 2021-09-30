/** @file tanh.hpp
    @brief Provides CORDIC for tanh function as a ratio of sinh and tanh
    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)

    @note H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/
#ifndef INC_LIBQ_CORDIC_TANH_HPP_
#define INC_LIBQ_CORDIC_TANH_HPP_

#include "libq/type_promotion.hpp"

#include <algorithm>

namespace libq {
namespace details {

template <typename T>
class tanh_of
{
public:
    using promoted_type = T;
};

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
class tanh_of<libq::fixed_point<T, n, f, e, op, up> >
    : private libq::fixed_point<T, 0, f, e, op, up>
    , public type_promotion_base<
          libq::
              fixed_point<typename std::make_signed<T>::type, 0, f, e, op, up>,
          1u,
          0,
          0>
{};

}  // namespace details
}  // namespace libq

namespace std {

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    tanh(libq::fixed_point<T, n, f, e, op, up> _val) ->
    typename libq::details::tanh_of<
        libq::fixed_point<T, n, f, e, op, up> >::promoted_type
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using tanh_type = typename libq::details::tanh_of<Q>::promoted_type;

    auto a = std::sinh(_val);
    auto b = std::cosh(_val);

    // reduce a and b to [0, 1] interval
    auto        m = std::max(std::fabs(a), std::fabs(b));
    std::size_t shifts(0);

    for (; m >= 1.0; ++shifts) {
        libq::lift(m) >>= 1u;
    }

    libq::lift(a) >>= shifts;
    libq::lift(b) >>= shifts;

    using work_type = typename libq::Q<f, f, e, op, up>;
    auto const c = tanh_type(work_type(a) / work_type(b));

    return tanh_type(c);
}

}  // namespace std

#endif  // INC_LIBQ_CORDIC_TANH_HPP_
