/** @file asin.hpp
    @brief Provides CORDIC for tanh function as a ratio of sinh and tanh
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
    @remark See H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
    */
#ifndef INC_LIBQ_CORDIC_ASIN_HPP_
#define INC_LIBQ_CORDIC_ASIN_HPP_

#include "libq/CORDIC/lut/lut.hpp"
#include "libq/type_promotion.hpp"

#include <cassert>

namespace libq {
namespace details {
template <typename T>
class asin_of
{
public:
    using promoted_type = T;
};

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
class asin_of<libq::fixed_point<T, n, f, e, op, up> >
    : private libq::fixed_point<T, 0, f, e, op, up>
    , public type_promotion_base<libq::fixed_point<T, 0, f, e, op, up>,
                                 2u,
                                 0,
                                 0>
{};

}  // namespace details
}  // namespace libq

namespace std {

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    asin(libq::fixed_point<T, n, f, e, op, up> _val) ->
    typename libq::details::asin_of<
        libq::fixed_point<T, n, f, e, op, up> >::promoted_type
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using result_type = typename libq::details::asin_of<Q>::promoted_type;
    using lut_type =
        libq::cordic::lut<result_type::bits_for_fractional, result_type>;

    assert(("[std::asin] argument is not from [-1.0, 1.0]",
            std::fabs(_val) <= Q(1.0f)));

    if (std::fabs(_val) > Q(1.0f)) {
        throw std::logic_error("[std::asin] argument is out of range");
    }

    if (_val == Q(1.0f)) {
        return result_type::CONST_PI_2;
    }

    if (_val == Q(-1.0f)) {
        return -(result_type::CONST_PI_2);
    }

    if (_val == Q(0.0)) {
        return result_type::wrap(0);
    }

    static lut_type const angles = lut_type::circular();
    static lut_type const scales = lut_type::circular_scales();

    // rotation mode: see page 6
    // shift sequence is just 0, 1, ... (circular coordinate system)
    result_type x(1.0f), y(0.0), z(0.0);

#ifdef LOOP_UNROLLING
    auto const iteration_body = [&](std::size_t i) {
#else
    for (std::size_t i = 0u; i != f; ++i) {
#endif
        int sign = _val >= y ? (x < 0.0 ? -1 : +1) : (x < 0.0 ? +1 : -1);

        typename result_type::storage_type const store{x.value()};

        /// @todo Use '-='
        x = x - result_type::wrap(sign * (y.value() >> i));
        /// @todo Use '+='
        y = y + result_type::wrap(sign * (store >> i));
        /// @todo Use '+='
        z = sign > 0 ? z + angles[i] : z - angles[i];
        /// @todo Use '*='
        _val = _val * scales[i];
    };
#ifdef LOOP_UNROLLING
    libq::details::unroll(iteration_body,
                          0u,
                          libq::details::loop_size<f - 1>());
#endif

    if (z > result_type::CONST_PI_2) {
        return result_type::CONST_PI - z;
    } else if (z < -result_type::CONST_PI_2) {
        return -result_type::CONST_PI - z;
    } else {
        return z;
    }
}

}  // namespace std

#endif  // INC_LIBQ_CORDIC_ASIN_HPP_
