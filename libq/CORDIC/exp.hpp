/** @file exp.hpp
    @brief Provides CORDIC for exp function
    @note see C. Baumann, "A simple and fast look-up table method to compute the exp(x) and ln(x) functions", 2004

    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_CORDIC_EXP_HPP_
#define INC_LIBQ_CORDIC_EXP_HPP_

#include <limits>

namespace libq {
namespace details {

template<typename T>
class exp_of
{
public:
    using promoted_type = T;
};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class exp_of<libq::fixed_point<T, n, f, e, op, up> >
{
public:
    using promoted_type =
        libq::fixed_point<
        std::uintmax_t,
        std::numeric_limits<std::uintmax_t>::digits - f,
        f,
        e,
        op,
        up
        >;
};

}  // namespace details
}  // namespace libq

namespace std {

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::exp_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type
exp(libq::fixed_point<T, n, f, e, op, up> _val)
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using exp_type = typename libq::details::exp_of<Q>::promoted_type;

    using Qw = libq::Q<f, f, e, op, up>;
    using work_type =
        typename libq::details::type_promotion_base<Qw, 1u, 0, 0>::promoted_type;

    using lut_type = libq::cordic::lut<f, work_type>;

    // reduces argument to interval [0.0, 1.0]
    int power(0);
    typename libq::details::mult_of<Q, work_type>::promoted_type
        arg(_val * work_type::CONST_LOG2E);

    for (; arg >= exp_type(1.0); ++power) {
        /// @todo Use -= ?
        arg = arg - 1u;
    }

    for (; arg < exp_type(0.0); --power) {
        /// @todo Use += ?
        arg = arg + 1u;
    }

    static lut_type const pow2_lut = lut_type::pow2();
    exp_type result(1.0);
    work_type x(arg);

#ifdef LOOP_UNROLLING
    auto const iteration_body = [&](std::size_t i) {
#else
    for (std::size_t i = 0u; i != f; ++i) {
#endif
        work_type const pow2 = work_type::wrap(T(1u) << (f - i - 1u));

        if (x - pow2 >= work_type(0.0)) {
            /// @todo Use -= ?
            x = x - pow2;

            result *= pow2_lut[i];
        }
    };
#ifdef LOOP_UNROLLING
    libq::details::unroll(iteration_body, 0u, libq::details::loop_size<f - 1>());
#endif

    if (power >= 0) {
        libq::lift(result) <<= power;
    } else {
        libq::lift(result) >>= -power;
    }
    return result;
}

}  // namespace std

#endif  // INC_LIBQ_CORDIC_EXP_HPP_
