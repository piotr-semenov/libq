// fixed_point.hpp
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file fixed_point.hpp

 Provides the fixed-point numbers of arbitrary format.
*/

#ifndef INC_LIBQ_FIXED_POINT_HPP_
#define INC_LIBQ_FIXED_POINT_HPP_

#include "./quotient_of.hpp"
#include "./product_of.hpp"

#include "./diff_of.hpp"
#include "./sum_of.hpp"

#include "./static_pow.hpp"

#include "./as_native_proxy.hpp"

#include <boost/integer.hpp>
#include <boost/cstdint.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <boost/integer/integer_mask.hpp>
#include <boost/integer_traits.hpp>

#include <limits>
#include <stdexcept>

/// @brief fixed-point arithmetics stuff
/// @detailed it contains fixed-point arithmetics, inference tools for product/
/// quotient/summand types, CORDIC implementation for couple of widely-used elementary
/// functions
namespace libq {
namespace details {

template<typename T>
T id(T val){ return val; }

template<typename operand_type1, typename operand_type2>
class product
{
public:
    // in case if product is a closed operation
    static operand_type1 perform(operand_type1 const& a, operand_type2 const& b,
        boost::mpl::bool_<true>)
    {
        struct chooser
        {
            enum { value = std::numeric_limits<operand_type1>::is_signed };
        };
        typedef typename boost::mpl::if_<chooser, boost::intmax_t, boost::uintmax_t>::type
            max_type;
        static int const left_bits = std::numeric_limits<max_type>::digits -
            std::numeric_limits<operand_type1>::digits - (operand_type2::total -
            operand_type2::fractionals);
        static int const shift1 =
            (left_bits < 0 || left_bits > operand_type2::fractionals)?
            0 : (int(operand_type2::fractionals) - left_bits);

        // overflow is possible
        typename operand_type1::word_type const val(a.value() *
            (b.value() >> shift1));
        static int const shift2 = (left_bits > 0 && left_bits < operand_type2::fractionals)?
            left_bits : operand_type2::fractionals;

        return operand_type1::wrap(
            operand_type1::handle_overflow(val >> shift2,
            operand_type1::o_policy())
        );
    }

    // in case if product is not a closed operation
    static typename product_of<operand_type1, operand_type2>::type
        perform(operand_type1 const& a, operand_type2 const& b, boost::mpl::bool_<false>)
    {
        // overflow is impossible
        typedef product_of<operand_type1, operand_type2> info;
        typedef info::word_type word_type;

        word_type const val(word_type(a.value()) * word_type(b.value()));
        return info::type::wrap(val);
    }
};

template<typename operand_type1, typename operand_type2>
class division
{
public:
    // in case if division is a closed operation
    static operand_type1 perform(operand_type1 const& a, operand_type2 const& b,
        boost::mpl::bool_<true>)
    {
        struct chooser
        {
            enum { value = std::numeric_limits<operand_type1>::is_signed };
        };
        typedef typename boost::mpl::if_<chooser, boost::intmax_t, boost::uintmax_t>::type
            max_type;
        static size_t const left_bits = std::numeric_limits<max_type>::digits -
            std::numeric_limits<operand_type1>::digits;

        // otherflow, underflow are possible
        max_type const shifted = max_type(a.value()) << left_bits;
        max_type val(shifted / operand_type1(b).value());
        operand_type1::handle_underflow(a.value(), val, operand_type1::u_policy());

        static size_t const shift = (left_bits > operand_type1::fractionals)?
            (left_bits - operand_type1::fractionals) :
            (operand_type1::fractionals - left_bits);
        val = (left_bits > operand_type1::fractionals)? val >> shift : val << shift;

        return operand_type1::wrap(operand_type1::handle_overflow(
            val,
            operand_type1::o_policy())
        );
    }

    // in case division is not a closed operation
    static typename quotient_of<operand_type1, operand_type2>::type
        perform(operand_type1 const& a, operand_type2 const& b, boost::mpl::bool_<false>)
    {
        // overflow, underflow are impossible
        typedef quotient_of<operand_type1, operand_type2> info;
        typedef info::word_type word_type;

        word_type const shifted = word_type(a.value()) << operand_type2::total;
        word_type const val = shifted / word_type(b.value());

        return info::type::wrap(val);
    }
};
} // details

// TYPE SIGNESS MANAGEMENT
inline float s(float x){ return x; }
inline double s(double x){ return x; }

template<typename T, size_t n, size_t f, class op, class up>
inline typename fixed_point<T, n, f, op, up>::to_signed_type s(fixed_point<T, n, f, op, up> x)
{
    return typename fixed_point<T, n, f, op, up>::to_signed_type(x);
}

inline float u(float x){ return x; }
inline double u(double x){ return x; }

template<typename T, size_t n, size_t f, class op, class up>
inline typename fixed_point<T, n, f, op, up>::to_unsigned_type u(fixed_point<T, n, f, op, up> x)
{
    return typename fixed_point<T, n, f, op, up>::to_unsigned_type(x);
}

// OVERFLOW/UNDERFLOW CONTROL POLICIES
/// @brief class to define the "throw an exception" behaviour in case of
/// overflow/underflow events
class do_exception{};

/// @brief class to define the "do overflow" behaviour in case of overflow
/// events
class do_overflow{};

/// @brief class to define the "do underflow" behaviour in case of underflow
/// events
class do_underflow{};

// OPERATE WITH TYPE AS WITH NATIVE TYPE
template<typename T, size_t n, size_t f, class op, class up>
class as_native_proxy;

/// @brief just an id map in case of floating-point types
inline float& as_native(float& x){ return x; }
inline double& as_native(double& x){ return x; }

/// @brief in case of fixed-point number type
template<typename T, size_t n, size_t f, class op, class up>
inline as_native_proxy<T, n, f, op, up> as_native(fixed_point<T, n, f, op, up>& x)
{
    return as_native_proxy<T, n, f, op, up>(x);
}

// FORWARD DECLARATIONS FOR CLASS - TYPE PROMOTERS
template<typename T> class sin_of;
template<typename T> class cos_of;
template<typename T> class tan_of;

template<typename T> class asin_of;
template<typename T> class acos_of;
template<typename T> class atan_of;

template<typename T> class sinh_of;
template<typename T> class cosh_of;
template<typename T> class tanh_of;

template<typename T> class asinh_of;
template<typename T> class acosh_of;
template<typename T> class atanh_of;

template<typename T> class log_of;
template<typename T> class exp_of;
template<typename T> class sqrt_of;

/*!
 \brief fixed-point number and its arithmetics. It allows the formats like UQn.f for unsigned numbers and Qn.f for signed ones
 \tparam storate_type built-in integral type to represent a fixed-point number
 \tparam n word length excepting the sign bit if existed. Note, if storage_type is signed then bits count is (n+1)
 \tparam f number of fractional bits. Note, f can be greater than n. Note, the number of integer bits is just max(n-f, 0).
 \tparam op overflow policy (actions to do if overflow occurred)
 \tparam up underflow policy (actions to do if underflow occurred)
*/
template<typename storage_type, size_t n, size_t f, class op, class up>
class fixed_point
{
    static_assert(std::is_integral<storage_type>::value);
    static_assert(n <= std::numeric_limits<storage_type>::digits);

    typedef fixed_point<storage_type, n, f, op, up> this_class;

    typedef op o_policy;
    typedef up u_policy;

// POSITIVE/NEGATIVE OVERFLOW HANDLERS
    // handles positive/negative overflow event with exception raising
    template<typename T>
    static storage_type handle_overflow(T val, do_exception)
    {
        static std::overflow_error const positive("positive overflow");
        static std::overflow_error const negative("negative overflow");

        if (val > T(0) && boost::uintmax_t(val) > bounds::max) {
            throw positive;
        }

        if (is_signed && boost::intmax_t(val) < bounds::min) {
            throw negative;
        }

        return static_cast<word_type>(val);
    }

    // handles positive/negative overflow event
    template<typename T>
    static storage_type handle_overflow(T val, do_overflow)
    {
        word_type u(0);

        if (details::id(is_signed)) {
            if (val > boost::intmax_t(bounds::max)) {
                u = static_cast<word_type>((val & bounds::max) - bounds::max);
            }
            else {
                int const sign_bit(val > 0 ? 1 : -1);
                u = word_type(sign_bit * static_cast<word_type>((sign_bit * val) &
                    bounds::max));
            }
        }
        else {
            u = static_cast<word_type>(val & bounds::max);
        }

        return u;
    }

// UNDERFLOW HANDLERS
    // handles underflow event with exception raising
    template<typename U, typename V>
    static V handle_underflow(U, V become, do_underflow)
    {
        return become;
    }

    // handles underflow event
    template<typename U, typename V>
    static V handle_underflow(U was, V become, do_exception)
    {
        static std::underflow_error const underflow("underflow");

        if (was != U(0) && become == V(0)) {
            throw underflow;
        }

        return become;
    }

public:
    typedef this_class type;

    typedef storage_type word_type; ///< integral type below fixed-point logics
    static size_t const fractionals = f; ///< count of bits for fractional part
    static size_t const total = n; ///< fixed-point number total

    ///< scale factor in fixed-point arithmetics
    static boost::uintmax_t const scale = details::static_pow<2, fractionals>::value;
    static bool const is_signed = std::numeric_limits<word_type>::is_signed;

    /// @brief range bounds for storage_type
    class bounds
    {
    public:
        ///< range upper bound
        static boost::uintmax_t const max = (boost::integer_traits<word_type>::const_max >>
            (boost::integer_traits<word_type>::digits - total));

    private:
        // case of signed logics
        template<bool t>
        struct min_bound_traits
        {
            // it is two's complement logics
            static boost::intmax_t const value = -static_cast<boost::intmax_t>(max) - 1;
        };

        // case of unsigned logics
        template<>
        struct min_bound_traits<false>
        {
            static boost::intmax_t const value = word_type(0);
        };

    public:
        ///< range lower bound
        static boost::intmax_t const min = min_bound_traits<is_signed>::value;
    };

    explicit fixed_point()
        :    m_value(0){}

    template<typename T>
    explicit fixed_point(T value)
        :    m_value(
                handle_underflow(
                    convert_from(value),
                    handle_overflow(convert_from(value), op()),
                    up()
                )
             ){}

    fixed_point(this_class const& x)
        :    m_value(x.value()){}

    template<typename T, size_t n1, size_t f1, class op1, class up1>
    fixed_point(fixed_point<T, n1, f1, op1, up1> const& x)
        :    m_value(this_class::normalize<T, f1>(x.value())){}

    inline operator float() const{ return float(to_float()); }
    inline operator double() const{ return to_float(); }

    template<typename T>
    void operator =(T const x)
    {
        static_assert(std::is_arithmetic<T>::value));

        this->value(convert_from(x)); 
    }

    this_class& operator =(this_class const& x)
    {
        return this->value(x.value());
    }

    template<typename T, size_t n1, size_t f1, class op1, class up1>
    this_class& operator =(fixed_point<T, n1, f1, op1, up1> const& x)
    {
        return this->value(normalize<T, f1>(x.value()));
    }

    template<typename T, size_t n1, size_t f1, class op1, class up1>
    this_class& operator =(as_native_proxy<T, n1, f1, op1, up1> const& x)
    {
        return this->value(convert_from(x.value()));
    }

    /// @brief represents any arithmetic value in current fixed-point format
    template<typename T>
    static word_type convert_from(T const x)
    {
        static_assert(std::is_arithmetic<T>::value);

        if (x > 0.0) {
            return word_type(std::floor(double(x) * this_class::scale + 0.5));
        }

        return word_type(std::ceil(double(x) * this_class::scale - 0.5));
    }

    /// @brief normalizes any fixed-point format to current one
    template<typename T, size_t f1>
    static word_type normalize(T const x)
    {
        static_assert(std::is_integral<T>::value);

        static size_t const shifts = (fractionals > f1) ? (fractionals - f1) :
            (f1 - fractionals);

        if (details::id(fractionals > f1)) {
            boost::mpl::if_c<is_signed, boost::intmax_t, boost::uintmax_t>::type
                val(x);
            // overflow is possible
            val <<= shifts;

            return this_class::handle_overflow(static_cast<word_type>(val), op());
        }

        // overflow, underflow are possible
        return this_class::handle_overflow(
            handle_underflow(x, static_cast<word_type>(x >> shifts),
                up()),
            op()
        );
    }

    /// @brief converts fixed-point to float
    inline double to_float() const{ return static_cast<double>(value()) / this_class::scale; }

    /// @brief wraps specified integral value as "fixed-point instance"
    template<typename T>
    static this_class wrap(T const val)
    {
        static_assert(std::is_integral<T>::value);
        this_class x;

        x.value(word_type(val));
        return x;
    }

// ORDERING OPERATORS:
    /// @brief 'less' binary relation in case if one of operands is a fixed-point
    /// number.
    /// @detailed number to compare with has to be converted to a fixed-point number
    /// of current format firstly.
    template<typename T>
    bool operator <(T const& x) const{ return this->value() < this_class(x).value(); }

    /// @brief 'less-equal' binary relation in case if one of operands is a fixed-point
    /// number
    template<typename T>
    bool operator <=(T const& x) const{ return this->value() <= this_class(x).value(); }

    /// @brief 'greater' binary relation in case if one of operands is a fixed-point
    /// number
    template<typename T>
    bool operator >(T const& x) const{ return this->value() > this_class(x).value(); }

    /// @brief 'greater-equal' binary relation in case if one of operands is a fixed-point
    /// number
    template<typename T>
    bool operator >=(T const& x) const{ return this->value() >= this_class(x).value(); }

    /// @brief 'equal-to' binary relation in case if one of operands is a fixed-point
    /// number
    template<typename T>
    bool operator ==(T const& x) const{ return this->value() == this_class(x).value(); }

    /// @brief 'not-equal-to' binary relation in case if one of operands is a fixed-point
    /// number
    template<typename T>
    bool operator !=(T const& x) const{ return this->value() != this_class(x).value(); }

    /// @brief 'not equal to zero' binary relation for fixed-point numbers
    bool operator !() const{ return this->value() != word_type(0); }


// FIXED-POINT ARITHMETICS:
    /// @brief extends current type to signed one (adds signed bit if needed)
    typedef fixed_point<typename boost::make_signed<word_type>::type, total, fractionals,
        op, up> to_signed_type;

    /// @brief reduces current type to unsigned one (remove signed bit if needed)
    typedef fixed_point<typename boost::make_unsigned<word_type>::type, total, fractionals,
        op, up> to_unsigned_type;

    /// @brief result type for summation operation
    typedef typename sum_of<this_class>::type sum_type;

    /// @brief result type for summation operation
    typedef typename diff_of<this_class>::type diff_type;

    /// @brief result type for multiplication operation
    typedef typename product_of<this_class, this_class>::type product_type;

    /// @brief result type for division operation
    typedef typename quotient_of<this_class, this_class>::type quotient_type;

    /// @brief result type for std::log operation

// FIXED-POINT ARITHMETICS:
    /// @brief addition operation
    /// @detailed number to add has to be converted to a fixed-point number
    /// of current format firstly. Result will be of type(a)::sum_type.
    template<typename T>
    inline sum_type operator +(T const& x) const
    {
        typedef sum_of<this_class>::word_type type;

        // any overflow is impossible
        return sum_type::wrap(
            type(this->value()) + type(this_class(x).value())
        );
    }

    template<typename T>
    inline this_class& operator +=(T const& x)
    {
        // overflow is possible
        sum_type const val(*this + this_class(x));

        return this->value(this_class::handle_overflow(val.value(), op()));
    }

    /// @brief subtraction operation
    /// @detailed number to subtract has to be converted to a fixed-point number
    /// of current format firstly. Result will be of type(a)::diff_type.
    template<typename T>
    inline diff_type operator -(T const& x) const
    {
        typedef diff_of<this_class>::word_type type;

        // any overflow is impossible
        return diff_type::wrap(
            type(this->value()) - type(this_class(x).value())
        );
    }

    template<typename T>
    inline this_class& operator -=(T const& x)
    {
        // overflow is possible
        diff_type const val(*this - this_class(x));

        return this->value(this_class::handle_overflow(val.value(), op()));
    }

    /// @brief multiplication operation
    /// @detailed number to multiply has to be converted to a fixed-point number
    /// of current format firstly. Result will be of type(a)::product_type.
    template<typename T>
    inline typename product_of<this_class, T>::type operator *(T const& x) const
    {
        return details::product<this_class, this_class>::perform(
            *this,
            this_class(x),
            boost::mpl::bool_<product_of<this_class, this_class>::is_closed::value>()
        );
    }
    template<typename T1, size_t n1, size_t f1, class op1, class up1>
    inline typename product_of<this_class, fixed_point<T1, n1, f1, op1, up1> >::type
        operator *(fixed_point<T1, n1, f1, op1, up1> const& x) const
    {
        typedef fixed_point<T1, n1, f1, op1, up1> operand_type;

        return details::product<this_class, operand_type>::perform(
            *this,
            x,
            boost::mpl::bool_<product_of<this_class, operand_type>::is_closed::value>()
        );
    }
    template<typename T>
    inline this_class& operator *=(T const& x)
    {
        product_type const val(*this * this_class(x));

        return this->value(word_type(val.value()));
    }

    /// @brief division operation
    /// @detailed number to divide by has to be converted to a fixed-point number
    /// of current format firstly. Result will be of type(a)::quotient_type.
    template<typename T1, size_t n1, size_t f1, class op1, class up1>
    inline typename quotient_of<this_class, fixed_point<T1, n1, f1, op1, up1> >::type
        operator /(fixed_point<T1, n1, f1, op1, up1> const& x) const
    {
        typedef fixed_point<T1, n1, f1, op1, up1> operand_type;

        return details::division<this_class, operand_type>::perform(
            *this,
            x,
            boost::mpl::bool_<quotient_of<this_class, operand_type>::is_closed::value>()
        );
    }
    template<typename T>
    inline this_class& operator /=(T const& x)
    {
        quotient_type const val(*this / this_class(x));

        return this->value(word_type(val.value()));
    }

// UNARY OPERATORS:
    this_class operator -() const
    {
        if (!is_signed) {
            return this_class::wrap(bounds::max - value());
        }

        return this_class::wrap(-value());
    }

    word_type const& value() const{ return this->m_value; }

// USEFUL MATHEMATICAL CONSTANTS:
    /// @brief naming conventions:
    /// 1. CONST_2PI means 2*pi;
    /// 2. CONST_2_PI means 2/pi;
    /// 3. CONST PI_2 means pi/2.
    static this_class const CONST_E, CONST_LOG2E, CONST_1_LOG2E, CONST_LOG10E,
        CONST_LOG102, CONST_LN2, CONST_LN10, CONST_2PI, CONST_PI, CONST_PI_2,
        CONST_PI_4, CONST_1_PI, CONST_2_PI, CONST_2_SQRTPI, CONST_SQRT2,
        CONST_SQRT1_2, CONST_2SQRT2;

// TYPES FOR ELEMENTARY_FUNCTIONS:
    typedef typename log_of<this_class>::type log_type;
    typedef typename sqrt_of<this_class>::type sqrt_type;

    typedef typename sin_of<this_class>::type sin_type;
    typedef typename cos_of<this_class>::type cos_type;
    typedef typename tan_of<this_class>::type tan_type;

    typedef typename asin_of<this_class>::type asin_type;
    typedef typename acos_of<this_class>::type acos_type;
    typedef typename atan_of<this_class>::type atan_type;

    typedef typename exp_of<this_class>::type exp_type;
    typedef typename sinh_of<this_class>::type sinh_type;
    typedef typename cosh_of<this_class>::type cosh_type;

    typedef typename asinh_of<this_class>::type asinh_type;
    typedef typename acosh_of<this_class>::type acosh_type;

    typedef typename atanh_of<this_class>::type atanh_type;
    typedef typename tanh_of<this_class>::type tanh_type;

private:
    word_type m_value;
    this_class& value(word_type const x)
    {
        this->m_value = x;

        return *this;
    }

    friend class as_native_proxy<storage_type, n, f, op, up>;

    template<typename operand_type1, typename operand_type2>
    friend class details::product;

    template<typename operand_type1, typename operand_type2>
    friend class details::division;
};

/// @brief type class in case of signed integral storages
template<size_t n, size_t f>
struct S_fixed_point
{
    // boost::int_t takes into account a sign bit
    typedef fixed_point<typename boost::int_t<1u + n>::least, n, f, do_overflow,
        do_underflow> type;
};

template<size_t n, size_t f>
struct SO_fixed_point
{
    typedef fixed_point<typename boost::int_t<1u + n>::least, n, f, do_exception,
        do_underflow> type;
};

template<size_t n, size_t f>
struct SU_fixed_point
{
    typedef fixed_point<typename boost::int_t<1u + n>::least, n, f, do_overflow,
        do_exception> type;
};

template<size_t n, size_t f>
struct SOU_fixed_point
{
    typedef fixed_point<typename boost::int_t<1u + n>::least, n, f, do_exception,
        do_exception> type;
};

/// @brief type class in case of unsigned integral storages
template<size_t n, size_t f>
struct U_fixed_point
{
    // boost::uint_t does not take into account a sign bit
    typedef fixed_point<typename boost::uint_t<n>::least, n, f, do_overflow,
        do_underflow> type;
};

template<size_t n, size_t f>
struct UO_fixed_point
{
    // boost::uint_t does not take into account a sign bit
    typedef fixed_point<typename boost::uint_t<n>::least, n, f, do_exception,
        do_underflow> type;
};

template<size_t n, size_t f>
struct UU_fixed_point
{
    // boost::uint_t does not take into account a sign bit
    typedef fixed_point<typename boost::uint_t<n>::least, n, f, do_overflow,
        do_exception> type;
};

template<size_t n, size_t f>
struct UOU_fixed_point
{
    // boost::uint_t does not take into account a sign bit
    typedef fixed_point<typename boost::uint_t<n>::least, n, f, do_exception,
        do_exception> type;
};
} // libq

#include "./details/numeric_limits.inl"

// mathematical constants
#define MATH_CONSTANT(name, val) \
    template<typename T, size_t n, size_t f, class op, class up> \
    core::fixed_point<T, n, f, op, up> const core::fixed_point<T, n, f, op, up>::##name(val);

MATH_CONSTANT(CONST_E, 2.71828182845904523536)
MATH_CONSTANT(CONST_1_LOG2E, 0.6931471805599453)
MATH_CONSTANT(CONST_LOG2E, 1.44269504088896340736)
MATH_CONSTANT(CONST_LOG10E, 0.434294481903251827651)
MATH_CONSTANT(CONST_LOG102, 0.301029995663981195214)
MATH_CONSTANT(CONST_LN2, 0.693147180559945309417)
MATH_CONSTANT(CONST_LN10, 2.30258509299404568402)
MATH_CONSTANT(CONST_2PI, 6.283185307179586)
MATH_CONSTANT(CONST_PI, 3.14159265358979323846)
MATH_CONSTANT(CONST_PI_2, 1.57079632679489661923)
MATH_CONSTANT(CONST_PI_4, 0.785398163397448309616)
MATH_CONSTANT(CONST_1_PI, 0.318309886183790671538)
MATH_CONSTANT(CONST_2_PI, 0.636619772367581343076)
MATH_CONSTANT(CONST_2_SQRTPI, 1.12837916709551257390)
MATH_CONSTANT(CONST_SQRT2, 1.41421356237309504880)
MATH_CONSTANT(CONST_SQRT1_2, 0.707106781186547524401)
MATH_CONSTANT(CONST_2SQRT2, 2.82842712474619009760)

#undef MATH_CONSTANT

// redefinition for stuff from std namespace
#include "./details/ceil.inl"
#include "./details/fabs.inl"
#include "./details/floor.inl"
#include "./details/fmod.inl"
#include "./details/round.inl"

// CORDIC-based fixed-point implementation of elementary functions
#include "./CORDIC/lut/lut.hpp"

#include "./CORDIC/cos.inl"
#include "./CORDIC/sin.inl"
#include "./CORDIC/tan.inl"
#include "./CORDIC/log.inl"
#include "./CORDIC/exp.inl"
#include "./CORDIC/acos.inl"
#include "./CORDIC/asin.inl"
#include "./CORDIC/atan.inl"
#include "./CORDIC/sqrt.inl"
#include "./CORDIC/sinh.inl"
#include "./CORDIC/cosh.inl"
#include "./CORDIC/tanh.inl"
#include "./CORDIC/asinh.inl"
#include "./CORDIC/acosh.inl"
#include "./CORDIC/atanh.inl"

#endif // INC_LIBQ_FIXED_POINT_HPP_
