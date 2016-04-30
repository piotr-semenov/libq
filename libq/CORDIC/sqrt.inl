// sqrt.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file sqrt.inl

 Provides CORDIC for sqrt function
 \ref see see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_STD_SQRT_INL_
#define INC_STD_SQRT_INL_

namespace libq {
namespace details {

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class sqrt_of
{
    enum: std::size_t {
        bits_for_integral = (n & 1u) ? (n / 2u + 1u) : (n / 2u),
        bits_for_fractional = (f & 1u) ? (f / 2u + 1u) : (f / 2u),

        number_of_significant_bits = bits_for_integral + bits_for_fractional
    };

    enum: int {
        scaling_factor_exponent = (e & 1u) ? (e / 2u + 1u) : (e / 2u)
    };

public:
    using promoted_storage_type = typename boost::uint_t<number_of_significant_bits>::least;
    using promoted_type = libq::fixed_point<
        promoted_storage_type,
        bits_for_integral,
        bits_for_fractional,
        scaling_factor_exponent,
        op,
        up>;
};
} // details
} // libq

namespace std {

/*!
 \brief computes square root by CORDIC-algorithm
 \ref page 11
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::sqrt_of<T, n, f, e, op, up>::promoted_type
    sqrt(libq::fixed_point<T, n, f, e, op, up> const& _val)
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using sqrt_type = typename libq::details::sqrt_of<T, n, f, e, op, up>::promoted_type;

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

    using reduced_type = typename std::conditional<
        Q::bits_for_integral >= 2,
        Q,
        work_type
    >::type;

    int power(0);
    reduced_type arg(_val);

    while (arg >= reduced_type(2.0)) {
        libq::lift(arg) >>= 1u;
        power--;
    }
    while (arg < reduced_type(1.0)) {
        libq::lift(arg) <<= 1u;
        power++;
    }

    // CORDIC vectoring mode:
    lut_type const angles = lut_type::hyperbolic_wo_repeated_iterations();
    typename libq::UQ<f, f, e, op, up> const norm(lut_type::hyperbolic_scale_with_repeated_iterations(f));
    work_type x(work_type(arg) + 0.25), y(work_type(arg) - 0.25), z(arg);
    {
        std::size_t repeated(4u);
        std::size_t num(0);

        for (std::size_t i = 1u; i < f + 1u; ++i) {
            int const sign = ((x.value() < 0)? -1 : +1) * ((y.value() < 0)? -1 : +1);
            typename work_type::storage_type const store(x.value());
            x = x - work_type::wrap(sign * (y.value() >> (num + 1u)));
            y = y - work_type::wrap(sign * (store >> (num + 1u)));
            z = (sign > 0) ? z + angles[num] : z - angles[num];

            // repeat until convergence is reached
            if (i == repeated && i != n - 1) {
                int const sign = ((x.value() < 0)? -1 : +1) * ((y.value() < 0)? -1 : +1);
                typename work_type::storage_type const store(x.value());
                x = x - work_type::wrap(sign * (y.value() >> (num + 1u)));
                y = y - work_type::wrap(sign * (store >> (num + 1u)));
                z = (sign > 0) ? z + angles[num] : z - angles[num];

                i += 1u;
                repeated = 3u * repeated  + 1u;
            }

            num += 1u;
        }
    }

    reduced_type result(x / norm);
    if (power > 0) {
        libq::lift(result) >>= (power >> 1u);
        if (power & 1u) {
            result = result / reduced_type::CONST_SQRT2;
        }
    }
    else {
        std::size_t const p(-power);
        libq::lift(result) <<= (p >> 1u);
        if (p & 1u) {
            result = result * work_type::CONST_SQRT2;
        }
    }

    return sqrt_type(result);
}
} // std

#endif // INC_STD_SQRT_INL_
