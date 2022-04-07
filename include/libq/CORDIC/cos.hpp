/** @file cos.hpp
    @brief Provides CORDIC for cos function
    @note See H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_CORDIC_COS_HPP_
#define INC_LIBQ_CORDIC_COS_HPP_

#include "libq/CORDIC/lut/lut.hpp"

namespace libq {
namespace details {

template <typename T>
class cos_of
{
public:
    using promoted_type = T;
};

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
class cos_of<libq::fixed_point<T, n, f, e, op, up> >
    : public libq::details::sin_of<libq::fixed_point<T, n, f, e, op, up> >
{};

}  // namespace details
}  // namespace libq

namespace std {

template <typename T, std::size_t n, std::size_t f, int e, class op, class up>
auto
    cos(libq::fixed_point<T, n, f, e, op, up> _val) ->
    typename libq::details::cos_of<
        libq::fixed_point<T, n, f, e, op, up> >::promoted_type
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using cos_type = typename libq::details::cos_of<Q>::promoted_type;

    // convergence interval for CORDIC rotations is [-pi/2, pi/2].
    // So one has to map input angle to that interval
    Q   arg(0);
    int sign(-1);

    // put argument to [-pi, pi] interval (with change of sign for cos)
    {
        Q const x = Q::CONST_PI - std::fmod(_val, Q::CONST_2PI);

        // convert to interval [-pi/2, pi/2]
        if (x < -Q::CONST_PI_2) {
            arg = x + Q::CONST_PI;
            sign = 1;
        } else if (x > Q::CONST_PI_2) {
            arg = x - Q::CONST_PI;
            sign = 1;
        } else {
            arg = x;
        }
    }

    using lut_type = libq::cordic::lut<f, Q>;
    static lut_type const angles = lut_type::circular();

    // normalization factor: see page 10, table 24.1 and pages 4-5, equations
    // (5)-(6) factor converges to the limit 1.64676 very fast: it takes 8
    // iterations only. 8 iterations corresponds to precision of size 0.007812
    // for angle approximation
    static Q norm_factor(1.0 / lut_type::circular_scale(f));

    // rotation mode: see page 6
    // shift sequence is just 0, 1, ... (circular coordinate system)
    Q x(norm_factor), y(0.0), z(arg);
    Q x1, y1, z1;

#ifdef LOOP_UNROLLING
    auto const iteration_body = [&](std::size_t i) {
#else
    for (std::size_t i = 0u; i != f; ++i) {
#endif
        int const sign = (z > Q(0)) ? 1 : -1;

        Q const x_scaled = Q::wrap(sign * (x.value() >> i));
        Q const y_scaled = Q::wrap(sign * (y.value() >> i));

        x1 = Q(x - y_scaled);
        y1 = Q(y + x_scaled);
        z1 = Q(z - Q((sign > 0) ? angles[i] : -angles[i]));

        x = x1;
        y = y1;
        z = z1;
    };
#ifdef LOOP_UNROLLING
    libq::details::unroll(iteration_body,
                          0u,
                          libq::details::loop_size<f - 1>());
#endif

    return cos_type((sign > 0) ? x : -x);
}

}  // namespace std

#endif  // INC_LIBQ_CORDIC_COS_HPP_
