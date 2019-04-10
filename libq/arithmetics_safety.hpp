/** @file arithmetics_safety.hpp

    @copyright 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
    Distributed under the New BSD License. (See accompanying file LICENSE)

    Provides the run-time overflow/underflow safety checks for basic arithmetics operations.
*/

#ifndef INC_LIBQ_ARITHMETICS_SAFETY_HPP_
#define INC_LIBQ_ARITHMETICS_SAFETY_HPP_

#include <string>
#include <stdexcept>

namespace libq {

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class fixed_point;

class overflow_exception_policy
{
    using This_class = overflow_exception_policy;

public:
    static constexpr bool const does_throw = true;

    static void
        raise_event(std::string const& _msg = std::string{})
    {
        throw std::overflow_error(_msg);
    }
};

class underflow_exception_policy
{
    using This_class = underflow_exception_policy;

public:
    static constexpr bool const does_throw = true;

    static void
        raise_event(std::string const& _msg = std::string{})
    {
        throw std::underflow_error(_msg);
    }
};

class ignorance_policy
{
    using This_class = ignorance_policy;

public:
    static constexpr bool const does_throw = true;

    static void
        raise_event(std::string const& = std::string{})
    {}
};


namespace details {

template<typename T> class sum_traits;
template<typename T1, typename T2> class mult_of;
template<typename T1, typename T2> class div_of;


/** @defgroup arithmetics_safety The run-time overflow/underflow safety checks for basic arithmetics operations.

    @note Please, see INT32-C. Ensure that checked operations do not result in overflow.

 @{
*/

/// @brief Checks if the addition operation overflows.
template<typename T, std::size_t n, std::size_t f, int e, typename... Ps>
bool
does_add_overflow(libq::fixed_point<T, n, f, e, Ps...> const _x,
                  libq::fixed_point<T, n, f, e, Ps...> const _y)
{
    using value_type = fixed_point<T, n, f, e, Ps...>;
    using result_type = typename sum_traits<value_type>::promoted_type;

    auto const a = _x.value();
    auto const b = _y.value();

    return
        (b > 0 && a > result_type::largest_stored_integer - b) ||
        (b < 0 && a < result_type::least_stored_integer - b);
}

/// @brief Checks if the subtraction operation overflows.
template<typename T, std::size_t n, std::size_t f, int e, typename... Ps>
static bool
does_sub_overflow(libq::fixed_point<T, n, f, e, Ps...> const _x,
                  libq::fixed_point<T, n, f, e, Ps...> const _y)
{
    using value_type = libq::fixed_point<T, n, f, e, Ps...>;
    using result_type = typename sum_traits<value_type>::promoted_type;

    auto const a = _x.value();
    auto const b = _y.value();

    return
        (b > 0 && a < result_type::least_stored_integer + b) ||
        (b < 0 && a > result_type::largest_stored_integer + b);
}

/// @brief Checks if the multiplication operation overflows.
template<
    typename T1,
    std::size_t n1,
    std::size_t f1,
    typename T2,
    int e1,
    std::size_t n2,
    std::size_t f2,
    int e2,
    typename... Ps>
bool
does_mul_overflow(libq::fixed_point<T1, n1, f1, e1, Ps...> const _x,
                  libq::fixed_point<T2, n2, f2, e2, Ps...> const _y)
{
    using Q1 = libq::fixed_point<T1, n1, f1, e1, Ps...>;
    using Q2 = libq::fixed_point<T2, n2, f2, e2, Ps...>;

    using result_type = typename mult_of<Q1, Q2>::promoted_type;
    using result_storage_type = typename result_type::storage_type;


    result_storage_type const a = static_cast<result_storage_type>(_x.value());
    result_storage_type const b = static_cast<result_storage_type>(_y.value());

    return
        a > 0 && b > 0 && a > result_type::largest_stored_integer / b ||
        a > 0 && b < 0 && b < result_type::least_stored_integer / a ||
        a < 0 && b > 0 && a < result_type::least_stored_integer / b ||
        a < 0 && b < 0 && b < result_type::largest_stored_integer / a;
}


/// @brief Checks if the division operation overflows.
template<
    typename T1,
    std::size_t n1,
    std::size_t f1,
    int e1,
    typename T2,
    std::size_t n2,
    std::size_t f2,
    int e2,
    typename... Ps>
bool
does_div_overflow(libq::fixed_point<T1, n1, f1, e1, Ps...> const _x,
                  libq::fixed_point<T2, n2, f2, e2, Ps...> const _y)
{
    using Q1 = libq::fixed_point<T1, n1, f1, e1, Ps...>;
    using Q2 = libq::fixed_point<T2, n2, f2, e2, Ps...>;
    using result_type = typename div_of<Q1, Q2>::promoted_type;


    auto const a = _x.value();
    auto const b = _y.value();

    return
        b == 0 ||
        /// @bug implicit cast to bool
        result_type::is_signed * (a == result_type::least_stored_integer && b == -1);
}


template<typename T, std::size_t n, std::size_t f, int e, typename... Ps>
bool
does_unary_neg_overflow(libq::fixed_point<T, n, f, e, Ps...> const _x)
{
    using result_type = fixed_point<T, n, f, e, Ps...>;

    auto const a = _x.value();
    return
        result_type::is_signed && (a == result_type::least_stored_integer);
}

/// @} arithmetics_safety

}  // namespace details
}  // namespace libq

#endif  // INC_LIBQ_OVERFLOW_HANDLER_BASE_HPP_
