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

#include <cstdint>
#include <cmath>
#include <exception>
#include <type_traits>

#include "boost/integer.hpp"
#include "boost/integer/integer_mask.hpp"

#include "arithmetics_safety.hpp"
#include "type_promotion.hpp"

namespace libq {

/*!
 \brief provides a proxy class to deal with stored integer behind the fixed-point number
*/
template<typename T, std::size_t n, std::size_t f, class op, class up>
T& lift(fixed_point<T, n, f, op, up>& _x){ return _x.m_value; }

/*!
 \brief fixed-point number and its arithmetics. It extends the formats like UQm.f, Qm.f with fixed pre-scaling factor p,
 where \f$p = 2^{min(0, n - f)}\f$, \f$m = f + max(n - f, 0)\f$.
 \tparam value_type built-in integral type to represent a fixed-point number.
 \tparam n number of significant bits (total, it includes the integral and fractional parts except the sign bit).
 \remark note, if storage_type is signed then number of used bits is \f$(n + 1)\f$.
 \remake note, the supremum of \f$n\f$ is std::numeric_limits<std::uintmax_t>::digits in case of unsigned numbers and
 std::numeric_limits<std::intmax_t>::digits in case of signed numbers
 \tparam f fixed-point format exponent (it is like a logical number of fractional bits).
 \remark note, if \f$f \geq n\f$ then number has fixed pre-scaling factor \f$p = 2^{n - f}\f$.
 \remark note, the number of integer bits is \f$\max(n - f, 0)\f$.
 \remark so note, \f$n\f$ is a physical entity and \f$f\f$ is a logical one.
 \tparam op class specifying the actions to do if overflow occurred
 \tparam up class specifying the actions to do if underflow occurred

 \ref see http://en.wikipedia.org/wiki/Q_(number_format) for details
*/
template<typename value_type, std::size_t n, std::size_t f, class op, class up>
class fixed_point
{
    static_assert(std::is_integral<value_type>::value, "value_type must be of the built-in integral type like std::uint8_t");

    typedef fixed_point<value_type, n, f, op, up> this_class;
    typedef typename std::conditional<std::numeric_limits<value_type>::is_signed, std::intmax_t, std::uintmax_t>::type largest_type;

public:
    typedef this_class type;
    typedef op overflow_policy;
    typedef up underflow_policy;

    /// \brief the integral type behind the fixed-point object
    typedef value_type storage_type;

    enum: std::size_t {
        number_of_significant_bits = n, ///< total number of significant bits
        mantissa_length = f, ///< length of mantissa

        bits_for_fractional = (f >= n) ? n : (n - f), ///< queried number of bits to represent the fractional part of fixed-point number
        bits_for_integral = (n >= f) ? (n - f) : 0, ///< number of bits to represent the integer part of fixed-point number

        is_signed = std::numeric_limits<storage_type>::is_signed ///< checks if this fixed-point number is signed
    };

    /*!
     \brief gets the scale factor for fixed-point numbers of current format
    */
    inline static double scale_factor()
    {
        //static double const factor = std::exp2(static_cast<double>(this_class::mantissa_length));
        static double const factor = std::pow(2.0, static_cast<double>(this_class::mantissa_length));
        return factor;
    }

    static_assert(number_of_significant_bits <= std::numeric_limits<largest_type>::digits, "system has no integer type of such big word size");
    enum : std::uintmax_t {
        // some trick to handle the case n = max possible word size
        integer_bits_mask =
            (this_class::number_of_significant_bits > this_class::bits_for_fractional) ?
                2u * (
                    (std::uintmax_t(1) << (number_of_significant_bits - 1u)) -
                    (std::uintmax_t(1) << (bits_for_fractional - 1u))
                ) : 0,
        fractional_bits_mask = 2u * ((std::uintmax_t(1) << (this_class::bits_for_fractional - 1u)) - 1u)
    };
    // need to avoid the signed/unsigned mismatch in arithmetic safety checks
    enum: typename this_class::largest_type {
        /// \brief the value of underlying integer behind the fixed-point maximum number
        largest_stored_integer =
            boost::low_bits_mask_t<this_class::number_of_significant_bits>::sig_bits
    };

    /// \brief the minimum rational value represented with current fixed-point format
    static this_class largest()
    { 
        return this_class::make_fixed_point<typename this_class::largest_type>(this_class::largest_stored_integer);
    }
    static this_class least(){ return this_class::make_fixed_point(this_class::least_stored_integer); }

    /// \brief the value of underlying integer behind the fixed-point minimum number
    static std::intmax_t const least_stored_integer = is_signed*(-static_cast<std::intmax_t>(largest_stored_integer)-1);

    /// \brief the dynamic range (dB) of fixed-point number
    static double dynamic_range_db(){ return 20.0 * std::log10(static_cast<double>(largest_stored_integer)); }

    /// \brief the precision of the fixed-point number
    static double precision(){ return 1.0 / this_class::scale_factor(); }

    /// \brief signed version of fixed-point number type
    typedef fixed_point<
        typename std::make_signed<storage_type>::type,
        this_class::number_of_significant_bits,
        this_class::bits_for_fractional,
        overflow_policy, underflow_policy
    > to_signed_type;

    /// \brief unsigned version of fixed-point number type
    typedef fixed_point<
        typename std::make_unsigned<storage_type>::type,
        this_class::number_of_significant_bits,
        this_class::bits_for_fractional,
        overflow_policy,
        underflow_policy
    > to_unsigned_type;

    /// \brief interprets the input integer as fixed-point number of the current format
    template<typename T>
    static this_class make_fixed_point(T const& _val)
    {
        static_assert(std::is_integral<T>::value, "input param must be of the built-in integral type");

        // checks if the input integer is not within the available dynamic range
        // if raise_overflow_event does nothing then compiler will suppress this check
        if ((_val < 0 && _val < this_class::least_stored_integer) || 
            (_val > 0 && _val > this_class::largest_stored_integer)) {
            overflow_policy::raise_event();
        }

        this_class x;
        x.m_value = storage_type(_val);

        return x;
    }

    /// \brief creates the zero being of the current fixed-point format
    explicit fixed_point()
        :   m_value(0){}

    fixed_point(this_class const& _x)
        :   m_value(_x.value()){}

    fixed_point(this_class const&& _x)
        :   m_value(_x.value()){}

    /// \brief normalizes the input fixed-point number to the current format
    template<typename T1, std::size_t n1, std::size_t f1, typename op1, typename up1>
    fixed_point(fixed_point<T1, n1, f1, op1, up1> const& _x)
        :   m_value(
                this_class::normalize(
                    _x,
                    std::integral_constant<bool, (f1 > this_class::bits_for_fractional)>()
                )
            ){}

    /// \brief creates the fixed-point number from any arithmetic object
    template<typename T>
    explicit fixed_point(T const _value)
        :   m_value(
                this_class::calc_stored_integer_from(
                    _value,
                    std::integral_constant<bool, std::is_floating_point<T>::value>()
                )
            ){}

    /// \brief move assign operator in case of rvalue being of same fixed-point format
    this_class& operator =(this_class const& _x)
    {
        this->m_value = _x.value();

        return *this;
    }

    /// \brief assign operator in case of rvalue being of different fixed-point format
    template<typename T1, std::size_t n1, std::size_t f1, typename op1, typename up1>
    this_class& operator =(fixed_point<T1, n1, f1, op1, up1> const& _x)
    {
        return
            this->set_value(
                this_class::normalize(_x, std::integral_constant<bool, (f1 > this_class::bits_for_fractional)>())
            );
    }

    /// \brief assign operator in all other cases: rvalue is integer/floating-point number/arithmetic type
    template<typename T>
    void operator =(T const& _x)
    {
        static_assert(std::is_arithmetic<T>::value, "T must be of the arithmetic type");

        this->m_value = this_class::calc_stored_integer_from(_x, std::integral_constant<bool, std::is_floating_point<T>::value>());
    }

    /// \brief converts this fixed-point number to the single-precision floating-point number
    operator float() const{ return static_cast<float>(to_floating_point()); }

    /// \brief converts this fixed-point number to the double-precision floating-point number
    operator double() const{ return to_floating_point(); }

    /// \brief returns the stored integer
    storage_type value() const{ return this->m_value; }

#define cond_operator(op) \
    template<typename T> \
    bool operator op(T const& _x) const{ return this->value() op this_class(_x).value(); }

    cond_operator(<);
    cond_operator(<=);
    cond_operator(>);
    cond_operator(>=);
    cond_operator(==);
    cond_operator(!=);
#undef cond_operator

    bool operator !() const{ return this->value() == 0; }

    /*!
     widely-used constants
     naming conventions:
     1. CONST_2PI means 2*pi;
     2. CONST_2_PI means 2/pi;
     3. CONST PI_2 means pi/2.
    */
    static this_class const CONST_E, CONST_LOG2E, CONST_1_LOG2E, CONST_LOG10E,
        CONST_LOG102, CONST_LN2, CONST_LN10, CONST_2PI, CONST_PI, CONST_PI_2,
        CONST_PI_4, CONST_1_PI, CONST_2_PI, CONST_2_SQRTPI, CONST_SQRT2,
        CONST_SQRT1_2, CONST_2SQRT2;

    // arithmetics: summation
    template<typename T>
    inline typename libq::details::sum_of<this_class>::promoted_type
        operator +(T const& _x) const
    {
        typedef typename libq::details::sum_of<this_class>::promoted_type sum_type;
        typedef typename sum_type::storage_type word_type;

        this_class const converted(_x); // note, *this and converted have the same storage_type
        if (details::does_addition_overflow(*this, converted)) {
            overflow_policy::raise_event();
        }

        word_type const stored_integer = word_type(this->value()) + word_type(converted.value());
        return sum_type::make_fixed_point(stored_integer);
    }

    template<typename T>
    inline this_class& operator +=(T const& _x)
    {
        typedef typename libq::details::sum_of<this_class>::promoted_type sum_type;

        sum_type const result = *this + _x;
        return (*this = this_class::make_fixed_point(result.value()));
        //return this->set_value(result.value());
    }

    // arithmetics: subtraction
    template<typename T>
    typename libq::details::sum_of<this_class>::promoted_type
        operator -(T const& _x) const
    {
        typedef typename libq::details::sum_of<this_class>::promoted_type diff_type;
        typedef typename diff_type::storage_type word_type;

        this_class const converted(_x); // note, *this and converted have the same storage_type
        if (details::does_subtraction_overflow(*this, converted)) {
            overflow_policy::raise_event();
        }

        word_type const stored_integer = word_type(this->value()) - word_type(converted.value());
        return diff_type::make_fixed_point(stored_integer);
    }

    template<typename T>
    this_class operator -=(T const& _x)
    {
        this_class const converted(_x);
        if (details::does_subtraction_overflow(*this, converted)) {
            overflow_policy::raise_event();
        }

        return this->set_value(this->value() - converted.value());
    }

    // arithmetics: multiplication
    /*!
     \note you need not to do the proper promotion of fixed-point types by yourself.
     If no extra significant bits is available for the promoted type then operands are .
     In other words, std::common_type<L, R>::type is always equal to L
    */
    template<typename T1, std::size_t n1, std::size_t f1, typename op1, typename up1>
    typename libq::details::mult_of<this_class, libq::fixed_point<T1, n1, f1, op1, up1> >::promoted_type
        operator *(libq::fixed_point<T1, n1, f1, op1, up1> const& _x) const
    {
        typedef typename libq::fixed_point<T1, n1, f1, op1, up1> operand_type;
        typedef libq::details::mult_of<this_class, operand_type> promotion_traits;
        typedef typename promotion_traits::promoted_type result_type;

        if (details::does_multiplication_overflow(*this, _x)) {
            overflow_policy::raise_event();
        }

        // do the exact or approximate multiplication of fixed-point numbers
        return result_type::make_fixed_point(
            multiply(*this, _x, std::integral_constant<bool, promotion_traits::is_expandable>())
            );
    }

    template<typename T1, std::size_t n1, std::size_t f1, typename op1, typename up1>
    this_class operator *=(libq::fixed_point<T1, n1, f1, op1, up1> const& _x)
    {
        typedef typename libq::fixed_point<T1, n1, f1, op1, up1> operand_type;
        typedef typename libq::details::mult_of<this_class, operand_type>::promoted_type result_type;

        if (details::does_multiplication_overflow(*this, _x)) {
            overflow_policy::raise_event();
        }

        typedef typename result_type::storage_type word_type;
        return (*this = (*this) * _x);
    }

    // arithmetics: division
    template<typename T1, std::size_t n1, std::size_t f1, typename... Ps>
    typename libq::details::div_of<this_class, libq::fixed_point<T1, n1, f1, Ps...> >::promoted_type
        operator /(libq::fixed_point<T1, n1, f1, Ps...> const& _x) const
    {
        typedef typename libq::fixed_point<T1, n1, f1, Ps...> operand_type;
        typedef libq::details::div_of<this_class, operand_type> promotion_traits;
        typedef typename promotion_traits::promoted_type result_type;

        if (details::does_division_overflow(*this, _x)) {
            overflow_policy::raise_event();
        }

        // do the exact or approximate division of fixed-point numbers
        return result_type::make_fixed_point(
                divide(*this, _x, std::integral_constant<bool, promotion_traits::is_expandable>())
            );
    }

    template<typename T1, std::size_t n1, std::size_t f1, typename... Ps>
    this_class operator /=(libq::fixed_point<T1, n1, f1, Ps...> const& _x)
    {
        typedef typename libq::fixed_point<T1, n1, f1, Ps...> operand_type;
        typedef typename libq::details::div_of<this_class, operand_type>::promoted_type result_type;

        this_class const converted(_x);
        if (details::does_division_overflow(*this, converted)) {
            overflow_policy::raise_event();
        }

        typedef typename result_type::storage_type word_type;
        return this->set_value(
            (*this / converted).value()
        );
    }

    // unary operator
    this_class operator -() const
    {
        if (details::does_unary_negation_overflow(*this)) {
            overflow_policy::raise_event();
        }
        if (!this_class::is_signed) {
            return this_class::make_fixed_point(this_class::largest_stored_integer - this->value());
        }

        return this_class::make_fixed_point(-this->value());
    }

private:

    /// \brief represents the input object being of the arithmetic type to a fixed-point number.
    /// It uses the rounding-to-nearest logics
    // in case of floating-point types
    template<typename T>
    static storage_type calc_stored_integer_from(T const& _x, std::true_type)
    {
        if (_x > T(0)) {
            return static_cast<storage_type>(std::floor(double(_x) * this_class::scale_factor() + 0.5));
        }

        return static_cast<storage_type>(std::ceil(double(_x) * this_class::scale_factor() - 0.5));
    }

    // in case of integral types
    template<typename T>
    static storage_type calc_stored_integer_from(T const& _x, std::false_type)
    {
        return storage_type(_x) << this_class::bits_for_fractional;
    }

    // normalizes the input fixed-point number to one of the current format in case \f$n > n1\f$
    template<typename T1, std::size_t n1, std::size_t f1, typename... Ps>
    static storage_type normalize(fixed_point<T1, n1, f1, Ps...> const& _x, std::false_type)
    {
        return
            storage_type(_x.value()) << (this_class::bits_for_fractional - f1);
    }

    // normalizes the input fixed-point number to one of the current format in case \f$n < n1\f$
    template<typename T1, std::size_t n1, std::size_t f1, typename... Ps>
    static storage_type normalize(fixed_point<T1, n1, f1, Ps...> const& _x, std::true_type)
    {
        storage_type const value =
            static_cast<storage_type>(_x.value() >> (f1 - this_class::bits_for_fractional));
        if (_x.value() && !value) {
            underflow_policy::raise_event();
        }

        return value;
    }

    // division of fixed-point numbers: do the exact division of fixed-point numbers
    template<typename T1, std::size_t n1, std::size_t f1, typename T2, std::size_t n2, std::size_t f2, typename... Ps>
    inline static typename libq::details::div_of<
        libq::fixed_point<T1, n1, f1, Ps...>,
        libq::fixed_point<T2, n2, f2, Ps...>
    >::promoted_storage_type
        divide(libq::fixed_point<T1, n1, f1, Ps...> const& _x, libq::fixed_point<T2, n2, f2, Ps...> const& _y, std::true_type)
    {
        typedef typename libq::fixed_point<T1, n1, f1, Ps...> operand_type1;
        typedef typename libq::fixed_point<T2, n2, f2, Ps...> operand_type2;
        typedef typename libq::details::div_of<operand_type1, operand_type2>::promoted_storage_type word_type;

        return
            (static_cast<word_type>(_x.value()) << operand_type2::number_of_significant_bits) /
            static_cast<word_type>(_y.value());
    }
    // division of fixed-point numbers: 
    template<typename T1, std::size_t n1, std::size_t f1, typename T2, std::size_t n2, std::size_t f2, typename... Ps>
    inline static typename libq::details::div_of<
        libq::fixed_point<T1, n1, f1, Ps...>,
        libq::fixed_point<T2, n2, f2, Ps...>
    >::promoted_storage_type
        divide(libq::fixed_point<T1, n1, f1, Ps...> const& _x, libq::fixed_point<T2, n2, f2, Ps...> const& _y, std::false_type)
    {
        typedef typename libq::fixed_point<T1, n1, f1, Ps...> operand_type1;
        typedef typename libq::fixed_point<T2, n2, f2, Ps...> operand_type2;
        typedef typename libq::details::div_of<operand_type1, operand_type2>::promoted_storage_type word_type;

        typedef typename this_class::largest_type max_type;

        // approximate division algorithm:
        // calc the diff between the available bits to shift and the limit
        static std::size_t const left_bits = std::numeric_limits<max_type>::digits - std::numeric_limits<operand_type1>::digits;
        max_type const shifted = max_type(_x.value()) << left_bits;
        max_type value = shifted / operand_type1(_y).value();

        static std::size_t const shifts = (left_bits > operand_type1::bits_for_fractional) ?
            (left_bits - operand_type1::bits_for_fractional) :
            (operand_type1::bits_for_fractional - left_bits);
        value = (left_bits > operand_type1::bits_for_fractional) ? (value >> shifts) : (value << shifts);

        return word_type(value);
    }

    // does the exact multiplication of fixed-point numbers
    template<typename T1, std::size_t n1, std::size_t f1, typename T2, std::size_t n2, std::size_t f2, typename... Ps>
    inline static typename libq::details::mult_of<
        libq::fixed_point<T1, n1, f1, Ps...>,
        libq::fixed_point<T2, n2, f2, Ps...>
    >::promoted_storage_type
    multiply(libq::fixed_point<T1, n1, f1, Ps...> const& _x, libq::fixed_point<T2, n2, f2, Ps...> const& _y, std::true_type)
    {
        typedef typename libq::fixed_point<T1, n1, f1, Ps...> operand_type1;
        typedef typename libq::fixed_point<T2, n2, f2, Ps...> operand_type2;
        typedef typename libq::details::mult_of<operand_type1, operand_type2>::promoted_storage_type word_type;

        return static_cast<word_type>(_x.value()) * static_cast<word_type>(_y.value());
    }
    template<typename T1, std::size_t n1, std::size_t f1, typename T2, std::size_t n2, std::size_t f2, typename... Ps>
    inline static typename libq::details::mult_of<
        libq::fixed_point<T1, n1, f1, Ps...>,
        libq::fixed_point<T2, n2, f2, Ps...>
    >::promoted_storage_type
    multiply(libq::fixed_point<T1, n1, f1, Ps...> const& _x, libq::fixed_point<T2, n2, f2, Ps...> const& _y, std::false_type)
    {
        typedef typename libq::fixed_point<T1, n1, f1, Ps...> operand_type1;
        typedef typename libq::fixed_point<T2, n2, f2, Ps...> operand_type2;
        typedef typename libq::details::mult_of<operand_type1, operand_type2>::promoted_storage_type word_type;

        typedef typename this_class::largest_type max_type;

        // approximate algorithm idea: we truncate the fractional precision of 2nd operand
        // static std::size_t const left_bits = (std::numeric_limits<max_type>::digits - std::numeric_limits<operand_type1>::digits) -
        //     operand_type2::bits_for_integral;
        // static std::size_t const shifts1 = (left_bits < 0 || left_bits > operand_type2::bits_for_fractional) ?
        //     0 : static_cast<int>(operand_type2::bits_for_fractional) - left_bits;
        // static std::size_t const shifts2 = (left_bits > 0 && left_bits <= operand_type2::bits_for_fractional) ?
        //     left_bits : operand_type2::bits_for_fractional;
        // 
        // return word_type(_x.value() * (_y.value() >> shifts1)) >> shifts2;
        return word_type(_x.value()) * word_type(_y.value());
    }

    // converts fixed-point number to the floating-point number
    double to_floating_point() const{ return static_cast<double>(value()) / this_class::scale_factor(); }

    storage_type m_value;
    this_class& set_value(storage_type const _x)
    {
        if (_x < this_class::least_stored_integer || _x > this_class::largest_stored_integer) {
            overflow_policy::raise_event();
        }

        this->m_value = _x;
        return *this;
    }

    friend storage_type& lift<value_type, n, f, overflow_policy, underflow_policy>(this_class&);
};

/*!
*/
template<std::size_t n, std::size_t f, class op = libq::ignorance_policy, class up = libq::ignorance_policy>
using Q = libq::fixed_point<typename boost::int_t<n+1>::least, n, f, op, up>;

template<std::size_t n, std::size_t f, class op = libq::ignorance_policy, class up = libq::ignorance_policy>
using UQ = libq::fixed_point<typename boost::uint_t<n>::least, n, f, op, up>;

} // libq

// constant implementation
#define LIBQ_FIXED_POINT_CONSTANT(name, value) \
    template<typename T, std::size_t n, std::size_t f, class op, class up> \
    libq::fixed_point<T, n, f, op, up> const libq::fixed_point<T, n, f, op, up>::name(value);

LIBQ_FIXED_POINT_CONSTANT(CONST_E, 2.71828182845904523536)
LIBQ_FIXED_POINT_CONSTANT(CONST_1_LOG2E, 0.6931471805599453)
LIBQ_FIXED_POINT_CONSTANT(CONST_LOG2E, 1.44269504088896340736)
LIBQ_FIXED_POINT_CONSTANT(CONST_LOG10E, 0.434294481903251827651)
LIBQ_FIXED_POINT_CONSTANT(CONST_LOG102, 0.301029995663981195214)
LIBQ_FIXED_POINT_CONSTANT(CONST_LN2, 0.693147180559945309417)
LIBQ_FIXED_POINT_CONSTANT(CONST_LN10, 2.30258509299404568402)
LIBQ_FIXED_POINT_CONSTANT(CONST_2PI, 6.283185307179586)
LIBQ_FIXED_POINT_CONSTANT(CONST_PI, 3.14159265358979323846)
LIBQ_FIXED_POINT_CONSTANT(CONST_PI_2, 1.57079632679489661923)
LIBQ_FIXED_POINT_CONSTANT(CONST_PI_4, 0.785398163397448309616)
LIBQ_FIXED_POINT_CONSTANT(CONST_1_PI, 0.318309886183790671538)
LIBQ_FIXED_POINT_CONSTANT(CONST_2_PI, 0.636619772367581343076)
LIBQ_FIXED_POINT_CONSTANT(CONST_2_SQRTPI, 1.12837916709551257390)
LIBQ_FIXED_POINT_CONSTANT(CONST_SQRT2, 1.41421356237309504880)
LIBQ_FIXED_POINT_CONSTANT(CONST_SQRT1_2, 0.707106781186547524401)
LIBQ_FIXED_POINT_CONSTANT(CONST_2SQRT2, 2.82842712474619009760)

#undef LIBQ_FIXED_POINT_CONSTANT

#include "details/sum_of.inl"
#include "details/mult_of.inl"
#include "details/div_of.inl"

#include "details/sign.inl"

#include "details/ceil.inl"
#include "details/fabs.inl"
#include "details/floor.inl"
#include "details/round.inl"
#include "details/remainder.inl"
#include "details/fmod.inl"
#include "details/numeric_limits.inl"
#include "details/type_traits.inl"

#include "CORDIC/lut/lut.hpp"

#include "CORDIC/log.inl"
#include "CORDIC/sqrt.inl"

#include "CORDIC/sin.inl"
#include "CORDIC/cos.inl"
#include "CORDIC/tan.inl"

#include "CORDIC/exp.inl"

#include "CORDIC/sinh.inl"
#include "CORDIC/cosh.inl"
#include "CORDIC/tanh.inl"

#include "CORDIC/acos.inl"
#include "CORDIC/asin.inl"
#include "CORDIC/atan.inl"

#include "CORDIC/asinh.inl"
#include "CORDIC/acosh.inl"
#include "CORDIC/atanh.inl"

#endif // INC_LIBQ_FIXED_POINT_HPP_
