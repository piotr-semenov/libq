// type_promotion.hpp
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/**
 @file type_promotion.hpp

 Provides the run-time overflow/underflow safety checks for basic arithmetics operations.
*/

#ifndef INC_LIBQ_TYPE_PROMOTION_HPP_
#define INC_LIBQ_TYPE_PROMOTION_HPP_

#include <boost/integer.hpp>
#include <boost/mpl/eval_if.hpp>

#include <limits>
#include <type_traits>

namespace libq {
namespace details {

/**
 @brief
 @tparam T fixed-point type
 @tparam delta_n
 @tparam delta_f
*/
template<typename T, std::size_t delta_n, std::size_t delta_f, int delte_e>
class type_promotion_base {
 public:
    using promoted_type = T;
};

template<typename T, std::size_t _n, std::size_t _f, int _e, class op, class up, std::size_t delta_n, std::size_t delta_f, int delta_e>  // NOLINT
class type_promotion_base<libq::fixed_point<T, _n, _f, _e, op, up>, delta_n, delta_f, delta_e> {  // NOLINT
    using Q = libq::fixed_point<T, _n, _f, _e, op, up>;
    using This_class = type_promotion_base<Q, delta_n, delta_f, delta_e>;

    using max_type = typename std::conditional<Q::is_signed,
                                               std::intmax_t,
                                               std::uintmax_t>::type;
    enum: std::size_t {
        sign_bit = static_cast<std::size_t>(Q::is_signed),

        n = Q::bits_for_integral,
        f = Q::bits_for_fractional
    };
    enum: int {
        e = Q::scaling_factor_exponent
    };

    // simple "type" wrapper for lazy instantiation of its "internal" type
    struct storage_type_promotion_traits {
        // Note, boost::int_t takes a sign bit into account
        using type = typename std::conditional<Q::is_signed,
                                               typename boost::int_t<(n + delta_n) + (f + delta_f) + This_class::sign_bit>::least,  // NOLINT
                                               typename boost::uint_t<(n + delta_n) + (f + delta_f)>::least>::type;  // NOLINT
    };
    struct storage_type_default_traits {
        using type = typename Q::storage_type;
    };

 public:
    enum: bool {
        is_expandable = ((n + delta_n) + (f + delta_f) <=
                            std::numeric_limits<max_type>::digits)
    };

    /**
     @brief
    */
    using promoted_storage_type =
        typename boost::mpl::eval_if_c<This_class::is_expandable,
                                       typename This_class::storage_type_promotion_traits,  // NOLINT
                                       typename This_class::storage_type_default_traits>::type;  // NOLINT

    using promoted_type = typename std::conditional<This_class::is_expandable,
        libq::fixed_point<promoted_storage_type, n + delta_n, f + delta_f, e + delta_e, typename Q::overflow_policy, typename Q::underflow_policy>,  // NOLINT
        libq::fixed_point<promoted_storage_type, n, f, e, typename Q::overflow_policy, typename Q::underflow_policy> >::type;  // NOLINT
};

}  // namespace details
}  // namespace libq
#endif  // INC_LIBQ_TYPE_PROMOTION_HPP_
