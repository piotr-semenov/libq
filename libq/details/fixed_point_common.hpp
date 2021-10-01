#ifndef INC_LIBQ_DETAILS_FIXED_POINT_HPP_
#define INC_LIBQ_DETAILS_FIXED_POINT_HPP_

#include "libq/arithmetics_safety.hpp"

#include "boost/integer/integer_mask.hpp"

#include <cstdint>
#include <cmath>
#include <exception>

#ifndef IMPLICIT_COPY_CTR
#define COPY_CTR_EXPLICIT_SPECIFIER explicit
#else
#define COPY_CTR_EXPLICIT_SPECIFIER
#endif

namespace libq {
namespace details {
inline double
    exp2(double _val)
{
    return std::exp2(_val);
}
}  // namespace details

/// @brief Gets the reference to the stored integer behind the fixed-point
/// number
template <typename T, std::size_t n, std::size_t f, int e, class... Ps>
T &
    /// @param[in] _x the fixed-point number
    lift(fixed_point<T, n, f, e, Ps...> &_x)
{
    return _x.m_value;
}

/// @brief Gets the value of the stored integer behind the fixed-point number
template <typename T, std::size_t n, std::size_t f, int e, class... Ps>
T const &
    /// @param[in] _x the fixed - point number
    lift(fixed_point<T, n, f, e, Ps...> const &_x)
{
    return _x.m_value;
}

/** @brief Implements the fixed-point number arithmetics.

    @note Note, this extends the Q-formats like
    UQn.f, Qn.f with fixed pre-scaling factor \f$2^e\f$.
    @tparam value_type Built-in integral type to represent a fixed-point number.
    @tparam n Number of integral bits.
    @tparam f Number of fractional bits.
    @remark Note, \f$n\f$ and \f$f\f$ exclude the sign bit.
    So if storage_type is signed then total number of bits is \f$(n + f + 1)\f$.
    @remark Note, the supremum of \f$(n + f)\f$ is
    std::numeric_limits<std::uintmax_t>::digits in case of the unsigned numbers
    and std::numeric_limits<std::intmax_t>::digits in case of the signed
    numbers.
    @tparam e Exponent of the pre-scaling factor \f$2^e\f$.
    @tparam op Policy class specifying the actions to do if overflow occurred.
    @tparam up Policy class specifying the actions to do if underflow occurred.

    <B>Usage</B>

    <I>Example 1</I>: Flexible switch between the floating-point calculations
    and fixed precision calculations while algorithm staying the same.
    @code{.cpp}
        #include "libq/fixed_point.hpp"
        #include <iostream>
        #include <cstdlib>

        namespace floating_point {
            using value_type = double;
        }  // namespace floating_point

        namespace fixed_precision {
            using value_type = libq::Q<30, 20>;
        }  // namespace fixed_precision

        using value_type = fixed_precision::value_type;

        int
            main(int, char**)
        {
            value_type input{ 0.0 };
            std::cin >> input;

            value_type const result = your_algorithm_here(input);
            std::cout << result << std::endl;

            return EXIT_SUCCESS;
        }
    @endcode

    @note Please, see http://en.wikipedia.org/wiki/Q_(number_format) for
    details.
*/
template <typename value_type,
          std::size_t n,
          std::size_t f,
          int         e,
          class op,
          class up>
class fixed_point
{
    static_assert(std::is_integral<value_type>::value,
                  "value_type must be of the built-in integral type");

    using This_class = fixed_point<value_type, n, f, e, op, up>;
    using largest_type =
        typename std::conditional<std::numeric_limits<value_type>::is_signed,
                                  std::intmax_t,
                                  std::uintmax_t>::type;

public:
    using type = This_class;
    using overflow_policy = op;
    using underflow_policy = up;

    /// @brief Used type for the stored integer.
    using storage_type = value_type;

    static constexpr int scaling_factor_exponent = e;

    /// @brief Total number of significant bits.
    static constexpr std::size_t const number_of_significant_bits = n + f;

    /// @brief Queried number of bits to represent the fractional part of
    /// fixed-point number.
    static constexpr std::size_t const bits_for_fractional = f;

    /// @brief Number of bits to represent the integral part.
    static constexpr std::size_t const bits_for_integral = n;

    /// @brief This checks if this fixed-point number is signed.
    static constexpr bool const is_signed =
        std::numeric_limits<storage_type>::is_signed;

    /// @brief Gets the scaling factor for this fixed-point number.
    static inline constexpr double
        scaling_factor()
    {
        return details::exp2(
            -static_cast<double>(This_class::scaling_factor_exponent));
    }

    static_assert(This_class::number_of_significant_bits <=
                      std::numeric_limits<largest_type>::digits,
                  "too big word size is required");

#define EXP2N(N) (std::uintmax_t(1u) << (N))

    /// @brief Scale factor for this fixed - point number.
    static constexpr std::uintmax_t const scale =
        EXP2N(This_class::bits_for_fractional);

    /// @brief Binary mask to extract the integral bits only from the stored
    /// integer. This is tricky to process if n + f = max. possible word size.
    static constexpr std::uintmax_t const integer_bits_mask =
        This_class::bits_for_integral > 0u
            ? 2u * (EXP2N(This_class::bits_for_fractional +
                          This_class::bits_for_integral - 1u) -
                    ((This_class::bits_for_fractional > 0u)
                         ? EXP2N(This_class::bits_for_fractional - 1u)
                         : 0u))
            : 0u;

    /// @brief Binary mask to extract the fractional bits from the stored
    /// integer.
    static constexpr std::uintmax_t const fractional_bits_mask =
        This_class::bits_for_fractional > 0u
            ? 2u * (EXP2N(This_class::bits_for_fractional - 1u) - 1u) + 1u
            : 0u;
#undef EXP2N

    /// @brief The maximum value of stored integer for this fixed-point format.
    static typename This_class::largest_type const largest_stored_integer =
        boost::low_bits_mask_t<
            This_class::number_of_significant_bits>::sig_bits;

    /// @brief Gets the maximum available fixed-point number.
    static This_class
        largest()
    {
        return This_class::wrap<typename This_class::largest_type>(
            This_class::largest_stored_integer);
    }

    /// @brief The minimum value of stored integer for this fixed-point format.
    static std::intmax_t const least_stored_integer =
        !!This_class::is_signed *
        (-static_cast<std::intmax_t>(This_class::largest_stored_integer) - 1);

    /// @brief Gets the minimum available fixed-point number.
    static This_class
        least()
    {
        return This_class::wrap(This_class::least_stored_integer);
    }

    /// @brief Gets the dynamic range (dB) for this fixed-point format
    static double
        dynamic_range_db()
    {
        static double const max_stored_integer =
            static_cast<double>(This_class::largest_stored_integer);

        return 20.0 * std::log10(max_stored_integer);
    }

    /// @brief Gets the precision of this fixed-point number.
    static double
        precision()
    {
        return 1.0 / This_class::scale;
    }

    /// @brief Signed version of this fixed-point number type.
    using to_signed_type =
        fixed_point<typename std::make_signed<storage_type>::type,
                    This_class::bits_for_integral,
                    This_class::bits_for_fractional,
                    This_class::scaling_factor_exponent,
                    overflow_policy,
                    underflow_policy>;

    /// @brief Unsigned version of this fixed-point number type.
    using to_unsigned_type =
        fixed_point<typename std::make_unsigned<storage_type>::type,
                    This_class::bits_for_integral,
                    This_class::bits_for_fractional,
                    This_class::scaling_factor_exponent,
                    overflow_policy,
                    underflow_policy>;

    /** @brief Wraps the input integer _val as a fixed-point number.

    <B>Usage</B>

    <I>Example 1</I>:
    @code{.cpp}
    #include "fixed_point.hpp"
    #include <cstdint>

    int
        main(int, char**)
    {
        using Q = libq::Q<30, 20>;

        std::uint8_t input{ 23u };
        Q const fp = Q::wrap(input);

        return EXIT_SUCCESS;
    }
    @endcode
    */
    template <typename T>
    static This_class
        wrap(T const &_val)
    {
        static_assert(std::is_integral<T>::value,
                      "input param must be of the built-in integral type");

        if ((_val < 0 &&
             intmax_t(_val) < intmax_t(This_class::least_stored_integer)) ||
            (_val > 0 &&
             uintmax_t(_val) > uintmax_t(This_class::largest_stored_integer))) {
            overflow_policy::raise_event();
        }

        This_class x;
        x.m_value = storage_type(_val);

        return x;
    }

    static This_class
        wrap(float const &) = delete;

    static This_class
        wrap(double const &) = delete;

    fixed_point() = default;
    fixed_point(This_class const &_x) = default;

    /// @brief Normalizes the input fixed-point number to be accepted by current
    /// format.
    template <typename T1,
              std::size_t n1,
              std::size_t f1,
              int         e1,
              typename op1,
              typename up1>
    COPY_CTR_EXPLICIT_SPECIFIER
        fixed_point(fixed_point<T1, n1, f1, e1, op1, up1> const &_x)
        : m_value(This_class::normalize(
              _x,
              std::integral_constant<bool,
                                     (int(f1) + e1 -
                                          int(This_class::bits_for_fractional) -
                                          This_class::scaling_factor_exponent >
                                      0)>{}))
    {}

    /// @brief Creates the fixed-point number from any arithmetic object.
    template <typename T>
    COPY_CTR_EXPLICIT_SPECIFIER
        fixed_point(T const &_value)
        : m_value(This_class::calc_stored_integer_from(
              _value,
              std::integral_constant<bool, std::is_floating_point<T>::value>{}))
    {}

    /// @brief Assigns the fixed-point number being of different format.
    template <typename T1,
              std::size_t n1,
              std::size_t f1,
              int         e1,
              typename op1,
              typename up1>
    This_class &
        operator=(fixed_point<T1, n1, f1, e1, op1, up1> const &_x)
    {
        using status_type =
            std::integral_constant<bool,
                                   (static_cast<int>(f1) + e1 >
                                    static_cast<int>(
                                        This_class::bits_for_fractional) +
                                        This_class::scaling_factor_exponent)>;
        return this->set_value_to(This_class::normalize(_x, status_type()));
    }

    This_class &
        operator=(This_class const &_x) = default;

    /// @brief Assigns any arithmetic type.
    template <typename T>
    This_class &
        operator=(T const &_x)
    {
        static_assert(std::is_arithmetic<T>::value,
                      "T must be of the arithmetic type");

        using status_type =
            std::integral_constant<bool, std::is_floating_point<T>::value>;

        this->m_value = This_class::calc_stored_integer_from(_x, status_type());

        return *this;
    }

    /// @brief Converts this fixed-point number to the single-precision
    /// floating-point number.
    explicit operator float() const
    {
        return static_cast<float>(to_floating_point());
    }

    /// @brief Converts this fixed-point number to the double-precision
    /// floating-point number.
    operator double() const
    {
        return to_floating_point();
    }

    /// @brief Gets the stored integer behind this fixed-point number.
    storage_type
        value() const
    {
        return this->m_value;
    }

    // Unfortunately, nobody can use BOOST.Operators here because it cannot
    // handle the template operators
#define COMPARISON_OPERATOR(op)                         \
    template <typename T>                               \
    bool operator op(T const &_x) const                 \
    {                                                   \
        return this->value() op This_class(_x).value(); \
    }

    COMPARISON_OPERATOR(<);
    COMPARISON_OPERATOR(<=);
    COMPARISON_OPERATOR(>);
    COMPARISON_OPERATOR(>=);
    COMPARISON_OPERATOR(==);
    COMPARISON_OPERATOR(!=);
#undef COMPARISON_OPERATOR

    bool
        operator!() const
    {
        return 0 == this->value();
    }

    /** @brief Fixed-point approximation of the widely-used constants.
        @note This uses the following naming convention:
        - CONST_2PI is for \f$2 * \pi\f$.
        - CONST_2_PI is for \f$\frac{2}{\pi}\f$.
        - CONST_PI_2 is for \f$\frac{\pi}{2}\f$.
        @note Weirdly, g++ 5.3.0 needs the full specification of type here.
        Otherwise, it will not interpret lines 705-707 as declaration instead of
        definition.
    */
    typedef fixed_point<value_type, n, f, e, op, up> const_type;

    static const_type const CONST_E;
    static const_type const CONST_LOG2E;
    static const_type const CONST_1_LOG2E;
    static const_type const CONST_LOG10E;
    static const_type const CONST_LOG102;
    static const_type const CONST_LN2;
    static const_type const CONST_LN10;
    static const_type const CONST_2PI;
    static const_type const CONST_PI;
    static const_type const CONST_PI_2;
    static const_type const CONST_PI_4;
    static const_type const CONST_1_PI;
    static const_type const CONST_2_PI;
    static const_type const CONST_2_SQRTPI;
    static const_type const CONST_SQRT2;
    static const_type const CONST_SQRT1_2;
    static const_type const CONST_2SQRT2;

    /** @brief Calculates the sum of the current fixed-point number and some
        numeric object.
        @note If no extra significant bits is available for the promoted type
        then the result type is equal to std::common_type<L, R>::type = L.
    */
    template <typename T>
    typename libq::details::sum_traits<This_class>::promoted_type
        operator+(T const &_x) const
    {
        using sum_type =
            typename libq::details::sum_traits<This_class>::promoted_type;
        using word_type = typename sum_type::storage_type;

        This_class const converted(_x);

        if (details::does_add_overflow(*this, converted)) {
            overflow_policy::raise_event();
        }

        word_type const stored_integer =
            word_type(this->value()) + word_type(converted.value());
        return sum_type::wrap(stored_integer);
    }

    template <typename T>
    inline This_class &
        operator+=(T const &_x)
    {
        This_class const result(*this + _x);

        return this->set_value_to(result.value());
    }

    /** @brief Subtracts some numeric object from the current fixed-point
        number.
        @note If no extra significant bits is available for the promoted type
        then the result type is equal to std::common_type<L, R>::type = L.
    */
    template <typename T>
    typename libq::details::sum_traits<This_class>::promoted_type
        operator-(T const &_x) const
    {
        using diff_type =
            typename libq::details::sum_traits<This_class>::promoted_type;
        using word_type = typename diff_type::storage_type;

        This_class const converted(_x);

        if (details::does_sub_overflow(*this, converted)) {
            overflow_policy::raise_event();
        }

        word_type const stored_integer =
            word_type(this->value()) - word_type(converted.value());
        return diff_type::wrap(stored_integer);
    }

    template <typename T>
    This_class
        operator-=(T const &_x)
    {
        This_class const result(*this - _x);

        return this->set_value_to(result.value());
    }

    /** @brief Multiplies the current fixed-point number with some numeric
        object.
        @note If no extra significant bits is available for the promoted type
        then the result type is equal to std::common_type<L, R>::type = L.
    */
    template <typename T1,
              std::size_t n1,
              std::size_t f1,
              int         e1,
              class op1,
              class up1>
    auto
        operator*(libq::fixed_point<T1, n1, f1, e1, op1, up1> const &_x) const
        -> typename libq::details::mult_of<
            This_class,
            libq::fixed_point<T1, n1, f1, e1, op1, up1> >::promoted_type
    {
        using operand_type =
            typename libq::fixed_point<T1, n1, f1, e1, op1, up1>;
        using promotion_traits =
            libq::details::mult_of<This_class, operand_type>;
        using result_type = typename promotion_traits::promoted_type;
        using word_type = typename promotion_traits::promoted_storage_type;

        if (details::does_mul_overflow(*this, _x)) {
            overflow_policy::raise_event();
        }

        // do the exact/approximate multiplication of fixed-point numbers
        return result_type::wrap(static_cast<word_type>(this->value()) *
                                     static_cast<word_type>(_x.value()) >>
                                 (promotion_traits::is_expandable
                                      ? 0
                                      : operand_type::bits_for_fractional));
    }

    template <typename T1,
              std::size_t n1,
              std::size_t f1,
              int         e1,
              class op1,
              class up1>
    This_class
        operator*=(libq::fixed_point<T1, n1, f1, e1, op1, up1> const &_x)
    {
        This_class const result(*this * _x);

        return this->set_value_to(result.value());
    }

    /** @brief Divides the current fixed-point number with some numeric object.

        @note If no extra significant bits is available for the promoted type
        then the result type is equal to std::common_type<L, R>::type = L.
    */
    template <typename T1, std::size_t n1, std::size_t f1, int e1, class... Ps>
    auto
        operator/(libq::fixed_point<T1, n1, f1, e1, Ps...> const &_x) const ->
        typename libq::details::div_of<
            This_class,
            libq::fixed_point<T1, n1, f1, e1, Ps...> >::promoted_type
    {
        if (details::does_div_overflow(*this, _x)) {
            overflow_policy::raise_event();
        }

        using operand_type = typename libq::fixed_point<T1, n1, f1, e1, Ps...>;
        using promotion_traits =
            libq::details::div_of<This_class, operand_type>;
        using word_type = typename promotion_traits::promoted_storage_type;
        word_type const shifted = static_cast<word_type>(this->value())
                                  << operand_type::number_of_significant_bits;

        auto const un_shifted =
            static_cast<typename operand_type::storage_type>(
                shifted >> operand_type::number_of_significant_bits);

        if (!promotion_traits::is_expandable && _x.value() != un_shifted) {
            overflow_policy::raise_event();
        }

        using result_type = typename promotion_traits::promoted_type;
        return result_type::wrap(shifted / static_cast<word_type>(_x.value()));
    }

    template <typename T1, std::size_t n1, std::size_t f1, int e1, class... Ps>
    This_class
        operator/=(libq::fixed_point<T1, n1, f1, e1, Ps...> const &_x)
    {
        This_class const result(*this / _x);

        return this->set_value_to(result.value());
    }

    /// @brief Gets the negative value of the current fixed-point number.
    This_class
        operator-() const
    {
        if (details::does_unary_neg_overflow(*this)) {
            overflow_policy::raise_event();
        }

        return This_class::wrap(-this->value());
    }

private:
    /** @brief Represents some floating-point number as a fixed-point number.
        @note It uses the rounding-to-nearest logics in case of floating-point
        types.
    */
    template <typename T>
    static storage_type
        calc_stored_integer_from(T const &_x, std::true_type)
    {
        static double const scale =
            static_cast<double>(This_class::scaling_factor_exponent);

        double const value = static_cast<double>(_x) / details::exp2(scale);

        if (_x > T(0)) {
            storage_type const converted = static_cast<storage_type>(
                std::floor(value * This_class::scale + 0.5));

            if (converted < 0) {
                overflow_policy::raise_event();
            }

            return converted;
        }

        return static_cast<storage_type>(
            std::ceil(value * This_class::scale - 0.5));
    }

    /// @brief Represents some integral number as a fixed-point number.
    template <typename T>
    static storage_type
        calc_stored_integer_from(T const &_x, std::false_type)
    {
        static constexpr double const scale =
            static_cast<double>(This_class::scaling_factor_exponent);
        double const value = static_cast<double>(_x) / details::exp2(scale);
        return storage_type(value) << This_class::bits_for_fractional;
    }

    /** @brief Normalizes the input fixed-point number to one of the current
        format in case \f$f + e - f1 - e1 > 0\f$.
    */
    template <typename T1, std::size_t n1, std::size_t f1, int e1, class... Ps>
    static storage_type
        normalize(fixed_point<T1, n1, f1, e1, Ps...> const &_x, std::false_type)
    {
        static constexpr std::size_t const shifts =
            (static_cast<int>(This_class::bits_for_fractional) +
             This_class::scaling_factor_exponent) -
            (e1 + f1);
        static_assert(shifts <= CHAR_BIT * sizeof(uintmax_t), "Invalid shift");
        storage_type const normalized = storage_type(_x.value()) << shifts;

        typedef
            typename fixed_point<T1, n1, f1, e1, Ps...>::storage_type st_type;

        auto const un_shifted = static_cast<st_type>(normalized >> shifts);

        if (_x.value() != un_shifted) {
            overflow_policy::raise_event();
        }

        return normalized;
    }

    /**
    @brief Normalizes the input fixed-point number to one of the current
    format in case \f$f + e - f1 - e1 < 0\f$.
    */
    template <typename T1, std::size_t n1, std::size_t f1, int e1, class... Ps>
    static storage_type
        normalize(fixed_point<T1, n1, f1, e1, Ps...> const &_x, std::true_type)
    {
        static constexpr std::size_t const shifts =
            (static_cast<int>(e1) + f1) -
            (static_cast<int>(This_class::bits_for_fractional) +
             This_class::scaling_factor_exponent);
        storage_type const normalized =
            static_cast<storage_type>(_x.value() >> shifts);

        if (_x.value() && !normalized) {
            underflow_policy::raise_event();
        }

        return normalized;
    }

    /// @brief Converts the fixed-point number to the floating-point number.
    double
        to_floating_point() const
    {
        return This_class::scaling_factor() *
               static_cast<double>(this->value()) / This_class::scale;
    }

    storage_type m_value;

    /** @brief This also checks if the stored integer is within the range of
        current fixed-point number.
    */
    This_class &
        set_value_to(storage_type const _x)
    {
        if (_x < This_class::least_stored_integer ||
            _x > This_class::largest_stored_integer) {
            overflow_policy::raise_event();
        }

        this->m_value = _x;
        return *this;
    }

    friend storage_type &
        lift<value_type, n, f, e, overflow_policy, underflow_policy>(
            This_class &);
};

/// @brief Short-cut for the signed fixed-point with just 2 template parameters
/// n and f.
template <std::size_t n,
          std::size_t f,
          int         e = 0,
          class op = libq::ignorance_policy,
          class up = libq::ignorance_policy>
using Q =
    libq::fixed_point<typename boost::int_t<n + 1>::least, n - f, f, e, op, up>;

/**
@brief Short-cut for the unsigned fixed-point with just 2 template parameters
n and f.
*/
template <std::size_t n,
          std::size_t f,
          int         e = 0,
          class op = libq::ignorance_policy,
          class up = libq::ignorance_policy>
using UQ =
    libq::fixed_point<typename boost::uint_t<n>::least, n - f, f, e, op, up>;

#define CONSTANT(name, value)             \
    template <class T,                    \
              std::size_t n,              \
              std::size_t f,              \
              int         e,              \
              class op,                   \
              class up>                   \
    fixed_point<T, n, f, e, op, up> const \
        fixed_point<T, n, f, e, op, up>::name(value);

CONSTANT(CONST_E, M_E)
CONSTANT(CONST_1_LOG2E, 0.6931471805599453)
CONSTANT(CONST_LOG2E, M_LOG2E)
CONSTANT(CONST_LOG10E, M_LOG10E)
CONSTANT(CONST_LOG102, 0.301029995663981195214)
CONSTANT(CONST_LN2, M_LN2)
CONSTANT(CONST_LN10, M_LN10)
CONSTANT(CONST_2PI, (2.0 * M_PI))
CONSTANT(CONST_PI, M_PI)
CONSTANT(CONST_PI_2, M_PI_2)
CONSTANT(CONST_PI_4, M_PI_4)
CONSTANT(CONST_1_PI, M_1_PI)
CONSTANT(CONST_2_PI, M_2_PI)
CONSTANT(CONST_2_SQRTPI, M_2_SQRTPI)
CONSTANT(CONST_SQRT2, M_SQRT2)
CONSTANT(CONST_SQRT1_2, M_SQRT1_2)
CONSTANT(CONST_2SQRT2, (2.0 * M_SQRT2))

#undef CONSTANT
}  // namespace libq

#endif  // INC_LIBQ_DETAILS_FIXED_POINT_HPP_
