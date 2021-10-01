/** @file numeric_limits.hpp
    @brief Provides the partial specialization of numeric_limits template for
    fixed-point numbers.
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/

#ifndef INC_LIBQ_DETAILS_NUMERIC_LIMITS_HPP_
#define INC_LIBQ_DETAILS_NUMERIC_LIMITS_HPP_

#include "libq/details/fixed_point_common.hpp"

#define M_LOG10_2 0.301029995663981195214

namespace std {

template <typename T,
          std::size_t n,
          std::size_t f,
          int         e,
          typename op,
          typename up>
class numeric_limits<libq::fixed_point<T, n, f, e, op, up> >
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;

public:
    static std::float_denorm_style const has_denorm = std::denorm_absent;
    static bool const                    has_denorm_loss = false;

    static bool const has_infinity = false;
    static bool const has_quiet_NaN = false;
    static bool const has_signaling_NaN = false;

    static bool const is_bounded = true;
    static bool const is_exact = false;

    static const bool is_iec599 = false;
    static const bool is_integer = false;

    static bool const is_modulo = true;
    static bool const is_signed = Q::is_signed;
    static bool const is_specialized = true;

    static bool const tinyless_before = up::does_throw;
    static bool const traps = true;

    static std::float_round_style const round_style = std::round_to_nearest;

    static int const digits = Q::number_of_significant_bits;
    static int const digits10 =
        static_cast<int>(Q::number_of_significant_bits * M_LOG10_2);

    static int const max_exponent = static_cast<int>(n);
    static int const max_exponent10 =
        static_cast<int>(max_exponent * M_LOG10_2);
    static int const min_exponent = -static_cast<int>(f);
    static int const min_exponent10 =
        static_cast<int>(min_exponent * M_LOG10_2);
    static int const radix = 2;

    /// @brief minimum value that can be achieved by fixed-point type
    static Q
        min() throw()
    {
        return Q::least();
    }

    /// @brief maximum value that can be achieved by fixed-point type
    static Q
        max() throw()
    {
        return Q::largest();
    }

    /// @brief returns the machine epsilon, that is, the difference between
    /// 1.0 and the next value representable by the fixed-point type
    static Q
        epsilon() throw()
    {
        return Q::wrap(1u);
    }

    /// @brief the maximum rounding error for fixed-point type
    static Q
        round_error() throw()
    {
        return Q(0.5f);
    }

    static Q
        denorm_min() throw()
    {
        return Q::wrap(0);
    }

    static Q
        infinity() throw()
    {
        return Q::wrap(0);
    }

    static Q
        quiet_NaN() throw()
    {
        return Q(0);
    }

    static Q
        signaling_NaN() throw()
    {
        return Q(0);
    }
};
}  // namespace std

#undef M_LOG10_2
#endif  // INC_LIBQ_DETAILS_NUMERIC_LIMITS_HPP_
