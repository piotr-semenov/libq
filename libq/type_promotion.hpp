/** @file type_promotion.hpp
    @brief Provides the run-time overflow/underflow safety checks for basic
    arithmetics operations.
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
*/
#ifndef INC_LIBQ_TYPE_PROMOTION_HPP_
#define INC_LIBQ_TYPE_PROMOTION_HPP_

#include "libq/details/fixed_point_common.hpp"

#include "boost/integer.hpp"
#include "boost/mpl/eval_if.hpp"

#include <limits>
#include <type_traits>

namespace libq {
namespace details {

/**
    @tparam T fixed-point type
    @tparam delta_n
    @tparam delta_f
*/
template <typename T, std::size_t delta_n, std::size_t delta_f, int delta_e>
class type_promotion_base
{
public:
    using promoted_type = T;
};

template <typename T,
          std::size_t _n,
          std::size_t _f,
          int         _e,
          class op,
          class up,
          std::size_t delta_n,
          std::size_t delta_f,
          int         delta_e>
class type_promotion_base<libq::fixed_point<T, _n, _f, _e, op, up>,
                          delta_n,
                          delta_f,
                          delta_e>
{
    using Q = libq::fixed_point<T, _n, _f, _e, op, up>;
    using This_class = type_promotion_base<Q, delta_n, delta_f, delta_e>;

    using max_type = typename std::
        conditional<Q::is_signed, std::intmax_t, std::uintmax_t>::type;

    static constexpr std::size_t const sign_bit =
        static_cast<std::size_t>(!!Q::is_signed);
    static constexpr std::size_t const n = Q::bits_for_integral;
    static constexpr std::size_t const f = Q::bits_for_fractional;
    static constexpr int const         e = Q::scaling_factor_exponent;
    static constexpr auto const total_data_bits = (n + delta_n) + (f + delta_f);

    /// simple "type" wrapper for lazy instantiation of its "internal" type
    struct storage_type_promotion_traits
    {
        // Note, boost::int_t takes a sign bit into account
        using type = typename std::conditional<
            Q::is_signed,
            typename boost::int_t<total_data_bits + This_class::sign_bit>,
            typename boost::uint_t<total_data_bits>>::type::least;
    };

    struct storage_type_default_traits
    {
        using type = typename Q::storage_type;
    };

public:
    static constexpr bool const is_expandable =
        total_data_bits <= std::numeric_limits<max_type>::digits;

    using promoted_storage_type = typename boost::mpl::eval_if_c<
        This_class::is_expandable,
        typename This_class::storage_type_promotion_traits,
        typename This_class::storage_type_default_traits>::type;

private:
    template <std::size_t n, std::size_t f, int e>
    using promoted_type_base = libq::fixed_point<promoted_storage_type,
                                                 n,
                                                 f,
                                                 e,
                                                 typename Q::overflow_policy,
                                                 typename Q::underflow_policy>;

public:
    using promoted_type = typename std::conditional<
        This_class::is_expandable,
        promoted_type_base<n + delta_n, f + delta_f, e + delta_e>,
        promoted_type_base<n, f, e>>::type;
};

}  // namespace details
}  // namespace libq

#endif  // INC_LIBQ_TYPE_PROMOTION_HPP_
