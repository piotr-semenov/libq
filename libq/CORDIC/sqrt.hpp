/** @file sqrt.hpp
    @brief Provides CORDIC for sqrt function
    @note See H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_CORDIC_SQRT_HPP_
#define INC_LIBQ_CORDIC_SQRT_HPP_

namespace libq {
namespace details {

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class sqrt_of
{
    static constexpr std::size_t const bits_for_integral = 0 != (n & 1u) ? n / 2u + 1u : n / 2u;
    static constexpr std::size_t const bits_for_fractional = 0 != (f & 1u) ? f / 2u + 1u : f / 2u;
    static constexpr std::size_t const number_of_significant_bits = bits_for_integral + bits_for_fractional;
    static constexpr int const scaling_factor_exponent = 0 != (e & 1u) ? e / 2u + 1u : e / 2u;

public:
    using promoted_storage_type =
        typename boost::uint_t<number_of_significant_bits>::least;

    using promoted_type = libq::fixed_point<promoted_storage_type,
        bits_for_integral,
        bits_for_fractional,
        scaling_factor_exponent,
        op,
        up>;
};

}  // namespace details
}  // namespace libq

namespace std {

/** @brief computes square root by CORDIC-algorithm
    @note page 11
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::sqrt_of<T, n, f, e, op, up>::promoted_type
sqrt(libq::fixed_point<T, n, f, e, op, up> const& _val)
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using sqrt_type =
        typename libq::details::sqrt_of<T, n, f, e, op, up>::promoted_type;

    assert(("[std::sqrt] argument is negative", _val >= Q(0)));

    if (_val < Q(0)) {
        throw std::logic_error("[std::sqrt]: argument is negative");
    }

    if (_val == Q(0.0)) {
        return sqrt_type(0.0);
    }

    if (_val == Q(1.0)) {
        return sqrt_type(1.0);
    }

    // Work fixed-point format must have several bits to represent
    // lut. Also format must enable argument translating to interval [1.0, 2.0].
    // So format must reserve two bits at least for integer part.
    using work_type = libq::Q<f + 2u, f, e, op, up>;
    using lut_type = libq::cordic::lut<f, work_type>;

    using reduced_type =
        typename std::conditional<Q::bits_for_integral >= 2, Q, work_type>::type;

    int power(0);
    reduced_type arg(_val);

    for (; arg >= reduced_type(2.0); --power) {
        libq::lift(arg) >>= 1u;
    }

    for (; arg < reduced_type(1.0); ++power) {
        libq::lift(arg) <<= 1u;
    }

    // CORDIC vectoring mode:
    lut_type const angles = lut_type::hyperbolic_wo_repeated_iterations();
    typename libq::UQ<f, f, e, op, up> const
        norm{lut_type::hyperbolic_scale_with_repeated_iterations(f)};

    work_type x{work_type{arg} +0.25};
    work_type y{work_type{arg} -0.25};
    work_type z{arg};
    {
        std::size_t repeated{4u};
        std::size_t num{0};

#ifdef LOOP_UNROLLING
        auto const iteration_body = [&](std::size_t i) {
#else
        for (std::size_t i = 1u; i < f + 1u; ++i) {
#endif
            int const sign = ((x.value() < 0) ? -1 : +1) *
                ((y.value() < 0) ? -1 : +1);
            typename work_type::storage_type const store(x.value());
            x = x - work_type::wrap(sign * (y.value() >> (num + 1u)));
            y = y - work_type::wrap(sign * (store >> (num + 1u)));
            z = (sign > 0) ? z + angles[num] : z - angles[num];

            // repeat until convergence is reached
            if (i == repeated && i != n - 1) {
                int const sign =
                    (x.value() < 0 ? -1 : +1) *
                    (y.value() < 0 ? -1 : +1);

                typename work_type::storage_type const store{x.value()};

                x = x - work_type::wrap(sign * (y.value() >> (num + 1u)));
                y = y - work_type::wrap(sign * (store >> (num + 1u)));
                z = (sign > 0) ? z + angles[num] : z - angles[num];

                i += 1u;
                repeated = 3u * repeated + 1u;
            }

            num += 1u;
        };
#ifdef LOOP_UNROLLING
        libq::details::unroll(iteration_body,
                              0u,
                              libq::details::loop_size<f>());
#endif
    }

    reduced_type result(x / norm);
    if (power > 0) {
        libq::lift(result) >>= (power >> 1u);

        if (power & 1u) {
            /// @todo /= ?
            result = result / reduced_type::CONST_SQRT2;
        }
    } else {
        std::size_t const p(-power);
        libq::lift(result) <<= (p >> 1u);

        if (p & 1u) {
            /// @todo *= ?
            result = result * work_type::CONST_SQRT2;
        }
    }

    return sqrt_type(result);
}

}  // namespace std

#endif  // INC_LIBQ_CORDIC_SQRT_HPP_
