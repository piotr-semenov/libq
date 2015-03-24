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

#include <cmath>
#include <cstdint>

#include "type_promotion.hpp"
#include "arithmetics_safety.hpp"

namespace libq {

/*!
 \brief fixed-point number and its arithmetics. It extends the formats like UQm.f, Qm.f with fixed pre-scaling factor p,
 where \f$p = 2^{min(0, n - f)}\f$, \f$m = f + max(n - f, 0)\f$.
 \tparam value_type built-in integral type to represent a fixed-point number.
 \tparam n number of significant bits (except the sign bit).
 \remark note, if storage_type is signed then number of used bits is \f$(n + 1)\f$.
 \tparam f number of fractional bits.
 \remark note, if \f$f \geq n\f$ then number has fixed pre-scaling factor \f$p = 2^{n - f}\f$.
 \remark note, the number of integer bits is \f$\max(n - f, 0)\f$.
 \tparam overflow_policy class specifying the actions to do if overflow occurred
 \tparam underflow_policy class specifying the actions to do if underflow occurred

 \ref see http://en.wikipedia.org/wiki/Q_(number_format) for details
*/
template<typename value_type, std::size_t n, std::size_t f, class overflow_policy, class underflow_policy>
class fixed_point
    :   private overflow_policy, private underflow_policy
{
    static_assert(std::is_integral<value_type>::value, "value_type must be of the built-in integral type like std::uint8_t");

    typedef fixed_point<value_type, n, f, overflow_policy, underflow_policy> this_class;
    //using overflow_policy::raise_overflow_event;
    //using underflow_policy::raise_underflow_event;

public:
    typedef this_class type;
    typedef overflow_policy op;
    typedef underflow_policy up;

    /// \brief the integral type behind the fixed-point object
    typedef value_type storage_type;

    enum: std::size_t {
        bits_for_fractional = f, ///< number of bits to represent the fractional part of fixed-point number
        number_of_significant_bits = n, ///< total number of significant bits

        is_signed = std::numeric_limits<storage_type>::is_signed ///< checks if this fixed-point number is signed
    };

    enum: std::uintmax_t {
        scale = std::uintmax_t(1u) << bits_for_fractional, ///< scaling factor for fixed-point interval

        /// \brief the value of underlying integer behind the fixed-point maximum number
        largest_stored_integer = (std::uintmax_t(1u) << number_of_significant_bits) - 1u,

        integer_bits_mask = (std::uintmax_t(1u) << number_of_significant_bits) -
            (std::uintmax_t(1u) << bits_for_fractional),
        fractional_bits_mask = (std::uintmax_t(1u) << bits_for_fractional) - 1u
    };

    /// \brief the minimum rational value represented with current fixed-point format
    static this_class largest(){ return this_class::make_fixed_point(largest_stored_integer); }
    static this_class least(){ return this_class::make_fixed_point(least_stored_integer); }

    /// \brief the value of underlying integer behind the fixed-point minimum number
    static std::intmax_t const least_stored_integer = is_signed*(-static_cast<std::intmax_t>(largest_stored_integer)-1);

    /// \brief the dynamic range (dB) of fixed-point number
    static double dynamic_range_db(){ return 20.0 * std::log10(static_cast<double>(largest_stored_integer)); }

    /// \brief the precision of the fixed-point number
    static double precision(){ return 1.0 / this_class::scale; }

    /// \brief interprets the input integer as fixed-point number of the current format
    template<typename T>
    static this_class make_fixed_point(T const val)
    {
        static_assert(std::is_integral<T>::value, "input param must be of the built-in integral type");

        // checks if the input integer is not within the available dynamic range
        // if raise_overflow_event does nothing then compiler will suppress this check
        if (val < this_class::least_stored_integer || val > this_class::largest_stored_integer) {
            //!!! raise_overflow_event();
        }

        this_class x;
        x.m_value = storage_type(val);

        return x;
    }

    /// \brief creates the zero being of the current fixed-point format
    explicit fixed_point()
        :   m_value(0){}

    fixed_point(this_class const& _x)
        :   m_value(_x.value()){}

    /// \brief normalizes the input fixed-point number to the current format
    template<typename T1, std::size_t n1, std::size_t f1, typename op, typename up>
    explicit fixed_point(fixed_point<T1, n1, f1, op, up> const& _x)
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

    /// \brief assign
    this_class& operator =(this_class const& _x)
    {
        this->m_value = _x.value();

        return *this;
    }

    /// \brief assign
    template<typename T1, std::size_t n1, std::size_t f1, typename op, typename up>
    this_class& operator =(fixed_point<T1, n1, f1, op, up> const& _x)
    {
        return
            this->set_value(
                this_class::normalize(_x, std::integral_constant<bool, (f1 > this_class::bits_for_fractional)>())
            );
    }

    /// \brief assign
    template<typename T>
    void operator =(T const& _x)
    {
        static_assert(std::is_arithmetic<T>::value, "T must be of the arithmetic type");

        this->m_value = this_class::calc_stored_integer_from(_x, std::integral_constant<bool, std::is_floating_point<T>::value());
    }

    /// \brief converts this fixed-point number to the single-precision floating-point number
    operator float() const{ return static_cast<float>(to_floating_point()); }

    /// \brief converts this fixed-point number to the double-precision floating-point number
    operator double() const{ return to_floating_point(); }

    /// \brief
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

    bool operator !() const{ return this->value() != 0; }

    // type promotion
    typedef typename libq::details::sum_of<this_class>::promoted_type sum_type;
    typedef sum_type diff_type;
    typedef typename libq::details::mult_of<this_class, this_class>::promoted_type mult_type;
    typedef typename libq::details::div_of<this_class, this_class>::promoted_type div_type;

    // arithmetics: summation
    template<typename T>
    inline sum_type operator +(T const& _x) const
    {
        typedef typename sum_type::storage_type word_type;
        this_class const converted(_x); // note, *this and converted have the same storage_type
        if (details::does_addition_overflow(*this, converted)) {
            // raise
        }

        word_type const stored_integer = word_type(this->value()) + word_type(converted.value());
        return sum_type::make_fixed_point(stored_integer);
    }

    template<typename T>
    inline this_class& operator +=(T const& _x)
    {
        sum_type const result = *this + _x;
        return (*this = this_class::make_fixed_point(result.value()));
        //return this->set_value(result.value());
    }

    // arithmetics: subtraction
    template<typename T>
    diff_type operator -(T const& _x) const
    {
        typedef typename diff_type::storage_type word_type;
        this_class const converted(_x); // note, *this and converted have the same storage_type
        if (details::does_subtraction_overflow(*this, converted)) {
            // raise
        }

        word_type const stored_integer = word_type(_x.value()) - word_type(converted.value());
        return diff_type::make_fixed_point(stored_integer);
    }

    template<typename T>
    this_class operator -=(T const& _x)
    {
        this_class const converted(_x);
        if (details::does_subtraction_overflow(*this, converted)) {
            // raise
        }

        return this->set_value(this->value() - converted.value());
    }

    // arithmetics: multiplication
    template<typename T1, std::size_t n1, std::size_t f1, typename op, typename up>
    typename libq::details::mult_of<this_class, libq::fixed_point<T1, n1, f1, op, up> >::promoted_type
        operator *(libq::fixed_point<T1, n1, f1, op, up> const& _x) const
    {
        typedef typename libq::fixed_point<T1, n1, f1, op, up> operand_type;
        typedef typename libq::details::mult_of<this_class, operand_type>::promoted_type result_type;

        if (details::does_multiplication_overflow(*this, _x)) {
            // raise
        }

        typedef typename result_type::storage_type word_type;
        word_type const stored_integer = word_type(this->value()) * word_type(_x.value());
        return result_type::make_fixed_point(stored_integer);
    }

    template<typename T1, std::size_t n1, std::size_t f1, typename op, typename up>
    this_class operator *=(libq::fixed_point<T1, n1, f1, op, up> const& _x)
    {
        typedef typename libq::fixed_point<T1, n1, f1, op, up> operand_type;
        typedef typename libq::details::mult_of<this_class, operand_type>::promoted_type result_type;

        if (details::does_multiplication_overflow(*this, _x)) {
            // raise
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
        typedef typename result_type::storage_type word_type;

        if (details::does_division_overflow(*this, _x)) {
            // raise
        }

        // do the exact division of fixed-point numbers
        word_type stored_integer;
        if (promotion_traits::is_expandable) {
            stored_integer =
                (static_cast<word_type>(this->value()) << operand_type::number_of_significant_bits) /
                static_cast<word_type>(_x.value());
        }
        else { // do the approximate division aka multiplication by the reciprocal
            stored_integer =
                ((word_type(1u) << operand_type::number_of_significant_bits) / word_type(_x.value())) * // computes the reciprocal
                static_cast<word_type>(this->value());
        }
        return result_type::make_fixed_point(stored_integer);
    }

    template<typename T1, std::size_t n1, std::size_t f1, typename... Ps>
    this_class operator /=(libq::fixed_point<T1, n1, f1, Ps...> const& _x)
    {
        typedef typename libq::fixed_point<T1, n1, f1, Ps...> operand_type;
        typedef typename libq::details::div_of<this_class, operand_type>::promoted_type result_type;

        this_class const converted(_x);
        if (details::does_division_overflow(*this, converted)) {
            // raise
        }

        typedef typename result_type::storage_type word_type;
        return this->set_value(
            (*this / converted).value()
        );
    }

private:

    /// \brief represents the input object being of the arithmetic type to a fixed-point number.
    /// It uses the rounding-to-nearest logics
    // in case of floating-point types
    template<typename T>
    static storage_type calc_stored_integer_from(T const& _x, std::true_type)
    {
        if (_x > T(0)) {
            return static_cast<storage_type>(double(_x) * this_class::scale + 0.5);
        }

        return static_cast<storage_type>(double(_x) * this_class::scale - 0.5);
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
            _x.value() >> (f1 - this_class::bits_for_fractional);
        if (!value) {
            // raise underflow
        }

        return value;
    }

    // converts fixed-point number to the floating-point number
    double to_floating_point() const{ return static_cast<double>(value()) / this_class::scale; }

    storage_type m_value;
    this_class& set_value(storage_type const _x)
    {
        if (_x < this_class::least_stored_integer || _x > this_class::largest_stored_integer) {
            //!!! raise_overflow_event();
        }

        this->m_value = _x;
        return *this;
    }
};
} // libq

#include "./details/numeric_limits.inl"

#endif // INC_LIBQ_FIXED_POINT_HPP_
