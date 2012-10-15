/// @mainpage fixed-point template library for easy integral simulations of
/// floating-point algorithms

#ifndef INC_FIXED_POINT_NUMBER_HPP_
#define INC_FIXED_POINT_NUMBER_HPP_

#include "./../../fixed_point_lib/src/quotient_info.hpp"
#include "./../../fixed_point_lib/src/product_info.hpp"

#include "./../../fixed_point_lib/src/diff_info.hpp"
#include "./../../fixed_point_lib/src/sum_info.hpp"

#include "./../../fixed_point_lib/src/static_pow.hpp"

#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>

#include <boost/integer.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/cstdint.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <boost/integer_traits.hpp>

#include <boost/operators.hpp>

#include <limits>
#include <cmath>

/// @brief fixed-point arithmetics stuff
/// @detailed it contains fixed-point arithmetics, inference tools for product/
/// quotient/summand types, CORDIC implementation for couple of widely-used elementary
/// functions
namespace utils {
    namespace {
        namespace mpl = boost::mpl;
    }

    template<typename T, size_t n, size_t f>
    class as_native_proxy;

    /// @brief just an id map in case of floating-point types
    inline float& as_native(float& x){ return x; }
    inline double& as_native(double& x){ return x; }

    /// @brief in case of fixed-point number type
    template<typename T, size_t n, size_t f>
    inline as_native_proxy<T, n, f> as_native(number<T, n, f>& x)
    {
        return as_native_proxy<T, n, f>(x);
    }

    /// @brief fixed-point number and arithmetics
    /// @param storage_type built-in integral type representing a fixed-point number
    /// @param n bits total for fixed-point number without sign bit in any case:
    /// so if storage_type is signed, total is (n + 1). In other case it is n exactly.
    /// @param f count of bits for fractional part: f can be greater than n
    template<typename storage_type, size_t n, size_t f>
    class number
        :    public boost::totally_ordered<number<storage_type, n, f> >, // >, <=, >=, != in terms of <, == and !
             public boost::shiftable<number<storage_type, n, f>, size_t> // <<, >> in terms of =<< and >>=
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<storage_type>));

        BOOST_STATIC_ASSERT((f >= 0));
        BOOST_STATIC_ASSERT((n <= std::numeric_limits<storage_type>::digits));

        typedef number<storage_type, n, f> this_class;

    public:
        typedef this_class type;

        typedef storage_type value_type; ///< integral type below fixed-point logics
        static size_t const fractionals = f; ///< count of bits for fractional part
        static size_t const total = n; ///< fixed-point number total

        ///< scale factor in fixed-point arithmetics
        static boost::uintmax_t const scale = static_pow<2, fractionals>::value;
        static bool const is_signed = std::numeric_limits<value_type>::is_signed;

        /// @brief range bounds for storage_type
        class bounds
        {
        public:
            ///< range upper bound
            static boost::uintmax_t const max = (boost::integer_traits<value_type>::const_max >>
                (boost::integer_traits<value_type>::digits - total));

        private:
            // case of signed logics
            template<bool t>
            struct min_bound_traits
            {
                static boost::intmax_t const value = -static_cast<boost::intmax_t>(max) - 1;
            };

            // case of unsigned logics
            template<>
            struct min_bound_traits<false>
            {
                static boost::intmax_t const value = value_type(0);
            };

        public:
            ///< range lower bound
            static boost::intmax_t const min = min_bound_traits<is_signed>::value;

            /// @brief checks if any integral value belongs to range of current
            /// fixed-point format
            /// @throw boost::numeric::positive_overflow
            /// @throw boost::numeric::negative_overflow
            template<typename T>
            static value_type check(T val)
            {
                using boost::numeric::positive_overflow;
                using boost::numeric::negative_overflow;

                if (std::numeric_limits<T>::is_signed && boost::intmax_t(val) < bounds::min) {
                    throw negative_overflow();
                }
                if (val > 0 && boost::uintmax_t(val) > bounds::max) {
                    throw positive_overflow();
                }

                return static_cast<value_type>(val);
            }
        };

        explicit number()
            :    m_value(0){}

        template<typename T>
        explicit number(T value)
            :    m_value(convert_from(value)){}

        explicit number(this_class const& x)
            :    m_value(x.value()){}

        template<typename T, size_t n1, size_t f1>
        number(number<T, n1, f1> const& x)
            :    m_value(normalize<T, f1>(x.value())){}

        template<typename T>
        operator T() const
        {
            BOOST_STATIC_ASSERT((boost::is_arithmetic<T>::value));

            return boost::numeric_cast<T>(to_float()); 
        }

        template<typename T>
        void operator =(T const x)
        {
            BOOST_STATIC_ASSERT((boost::is_arithmetic<T>::value));

            this->value(convert_from(x)); 
        }

        this_class& operator =(this_class const& x)
        {
            return this->value(x.value());
        }

        template<typename T, size_t n1, size_t f1>
        this_class& operator =(number<T, n1, f1> const& x)
        {
            return this->value(normalize<T, f1>(x.value()));
        }

        /// @brief represents any arithmetic value in current fixed-point format
        template<typename T>
        static value_type convert_from(T const x)
        {
            BOOST_STATIC_ASSERT((boost::is_arithmetic<T>::value));

            return boost::numeric_cast<value_type>(x * this_class::scale);
        }

        /// @brief normalizes any fixed-point format to current one
        template<typename T, size_t f1>
        static value_type normalize(T const x);

        /// @brief converts fixed-point to float
        double to_float() const
        {
            return static_cast<double>(value()) / this_class::scale;
        }

        /// @brief wraps specified integral value as "fixed-point instance"
        template<typename T>
        static this_class wrap(T const val)
        {
            BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));
            this_class x;

            x.value(bounds::check(val));
            return x;
        }

    // ORDERING OPERATORS
        /// @brief 'less' binary relation for fixed-point numbers
        /// @param x instance to compare with
        bool operator <(this_class const& x) const;

        /// @brief 'equal-to' binary relation for fixed-point numbers
        /// @param x instance to compare with
        bool operator ==(this_class const& x) const;

        /// @brief 'not equal to zero' binary relation for fixed-point numbers
        bool operator !() const;

    // FIXED-POINT ARITHMETICS
        /// @brief extends current type to signed one (adds signed bit if needed)
        typedef number<typename boost::make_signed<value_type>::type, total, fractionals>
            to_signed_type;

        /// @brief reduces current type of unsigned one (remove signed bit if needed)
        typedef number<typename boost::make_unsigned<value_type>::type, total, fractionals>
            to_unsigned_type;

        /// @brief summation result type
        typedef typename sum_info<this_class>::sum_type sum_type;

        /// @brief subtraction result type
        typedef typename diff_info<this_class>::diff_type diff_type;

        /// @brief product result type
        typedef typename product_info<this_class>::product_type product_type;

        /// @brief division result type
        typedef typename quotient_info<this_class>::quotient_type quotient_type;

    private:
        static boost::uintmax_t const mod = static_pow<2, total - 1>::value;

        // FIXED-POINT ARITHMETICS AUX:
        // 1. summation in cases if it is not a closed operation and if it is
        template<typename T>
        inline sum_type add(this_class const& a, T const& b, mpl::bool_<true>::type) const;

        template<typename T>
        inline this_class add(this_class const& a, T const& b, mpl::bool_<false>::type) const;

        // 2. subtraction in cases if it is not a closed operation and if it is
        template<typename T>
        diff_type subtract(this_class const& a, T const& b, mpl::bool_<true>::type) const;

        template<typename T>
        this_class subtract(this_class const& a, T const& b, mpl::bool_<false>::type) const;

        // 3. multiplication in cases if it is not a closed operation and if it is
        template<typename T>
        product_type product(this_class const& a, T const& b, mpl::bool_<true>::type) const;

        template<typename T>
        this_class product(this_class const& a, T const& b, mpl::bool_<false>::type) const;

        // if product is closed operation: case of another fixed-point
        template<typename storage_type1, size_t total1, size_t fractionals1>
        this_class product(this_class const& a, number<storage_type1, total1, fractionals1> const& b, mpl::bool_<false>::type) const;

        // 4. division in cases if it is not a closed operation and if it is
        template<typename T>
        quotient_type divide(this_class const& a, T const &b, mpl::bool_<true>::type) const;

        template<typename T>
        this_class divide(this_class const& a, T const &b, mpl::bool_<false>::type) const;

        template<typename storage_type1, size_t total1, size_t fractionals1>
        this_class divide(this_class const& a, number<storage_type1, total1, fractionals1> const&b, mpl::bool_<false>::type) const;

    public:
        /// @brief addition with a number
        /// @detailed added number will be converted to fixed-point firstly.
        ///           Its format will be the same as the left operand has.
        ///           So expression (a + b) will be processed by the next algorithm:
        ///           1. b will be converted to value b' with fixed-point format of a
        ///           2. (a + b') will be performed
        ///           So, (a + b) will have the type type(a)::sum_type.
        ///           So we have, (signed + ...) -> signed, (unsigned + ...) -> unsigned.
        template<typename Other_type>
        sum_type operator +(Other_type const& x) const;

        /// @brief subtracts within a number
        /// @detailed subtracted number will be converted to fixed-point firstly.
        ///           Its format will be the same as the left operand has.
        ///           So expression (a - b) will be processed by the next algorithm:
        ///           1. b will be converted to value b' with fixed-point format of a
        ///           2. (a - b') will be performed
        ///           So, (a - b) will have the type type(a)::diff_type.
        ///           So we have, (signed - ...) -> signed, (unsigned - ...) -> unsigned.
        template<typename Other_type>
        diff_type operator -(Other_type const& x) const;

        /// @brief multiplies by a number
        /// @detailed multiplied number will be converted to fixed-point firstly.
        ///           Its format will be the same as the left operand has.
        ///           So expression (a * b) will be processed by the next algorithm:
        ///           1. b will be converted to value b' with fixed-point format of a
        ///           2. (a * b') will be performed
        ///           So, (a * b') will have the type type(a)::product_type.
        ///           So we have, (signed * ...) -> signed, (unsigned * ...) -> unsigned.
        template<typename Other_type>
        product_type operator *(Other_type const& x) const;

        /// @brief divides by a number that will be converting to fixed-point form firstly
        /// @detailed divider will be converted to fixed-point firstly.
        ///           Its format will be the same as the left operand has.
        ///           So expression (a / b) will be processed by the next algorithm:
        ///           1. b will be converted to value b' with fixed-point format of a
        ///           2. (a / b') will be performed
        ///           So, (a / b') will have the type type(a)::quotient_type.
        ///           So we have, (signed * ...) -> signed, (unsigned * ...) -> unsigned.
        template<typename Other_type>
        quotient_type operator /(Other_type const& x) const;

        //////////////////////////////////// OPERATIONS IN RING Z/Z_p ///////////////////////////////////
        /// @brief interprets fixed-point number as integer and adds to it an integer
        template<typename T>
        void operator +=(T x)
        {
            BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));

            this->value(this->value() + x);
        }

        /// @brief interprets fixed-point number as integer and subtracts an integer from it
        template<typename T>
        void operator -=(T x)
        {
            BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));

            this->value(this->value() - x);
        }

        /// @brief interprets fixed-point number as integer and multiplies it by an integer
        template<typename T>
        void operator *=(T x)
        {
            BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));

            this->value(this->value() * x);
        }

        /// @brief interprets fixed-point number as integer and multiplies it by an integer
        ///        as fixed-point
        template<typename storage_type1, size_t total1, size_t fractionals1>
        void operator *=(number<storage_type1, total1, fractionals1> const x)
        {
            this->value(this->value() * x.value());
        }

        /// @brief interprets fixed-point number as integer and divides it by an integer
        template<typename T>
        void operator /=(T x)
        {
            BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));

            this->value(this->value() / x);
        }

        ///////////////////////////// SHIFTING WITH TRUNCATION OPERATORS /////////////////
        this_class& operator >>=(size_t shifts){ this->value(this->value() >> shifts); return *this; }
        this_class& operator <<=(size_t shifts){ this->value(this->value() << shifts); return *this; }

        ///////////////////////////// UNARY OPERATORS ////////////////////////////////////
        this_class operator -() const{ return this_class::wrap(-value()); }

        value_type const& value() const{ return this->m_value; }

    private:
        value_type m_value;
        this_class& value(value_type const x)
        {
            this->m_value = x;

            return *this;
        }

        friend class as_native_proxy<storage_type, n, f>;
    };

    /// @brief just for convenience
    template<size_t total, size_t fractionals>
    struct S_number
    {
        // boost::int_t takes into account sign bit
        typedef number<typename boost::int_t<1 + total>::least, total, fractionals> type;
    };

    template<size_t total, size_t fractionals>
    struct U_number
    {
        typedef number<typename boost::uint_t<total>::least, total, fractionals> type;
    };
}

namespace std {
    namespace {
        using utils::number;

        using utils::static_pow;
    }

    /// @brief computes absolute value
    template<typename storage_type, size_t total, size_t fractionals>
    number<storage_type, total, fractionals> fabs(number<storage_type, total, fractionals> const& x)
    {
        static number<storage_type, total, fractionals> const zero(0.0);

        return (x < zero) ? -x : x;
    }

    /// @brief rounds
    template<typename storage_type, size_t total, size_t fractionals>
    number<storage_type, total, fractionals> round(number<storage_type, total, fractionals> const& x)
    {
        typedef number<storage_type, total, fractionals> number_type;

        static number_type const zero(0);
        static number_type const half(0.5);

        size_t const scaler = number_type::scale;

        number_type::value_type val = x.value() + ((x > zero) ? half.value() : -half.value());
        val >>= number_type::scale;
        val <<= number_type::scale;

        return number_type::wrap(val);
    }

    /// @brief computes fixed-point remainder of x/y
    template<typename storage_type, size_t total, size_t fractionals>
    number<storage_type, total, fractionals> fmod(number<storage_type, total, fractionals> const& a,
                                                  number<storage_type, total, fractionals> const& b)
    {
        return number<storage_type, total, fractionals>::wrap(a.value() % b.value());
    }

    /// @brief computes square root for fixed-point by long-algorithm
    template<typename storage_type, size_t total, size_t fractionals>
    number<storage_type, total, fractionals> sqrt(number<storage_type, total, fractionals> const& x)
    {
        typedef number<storage_type, total, fractionals> number_type;
        typedef number<storage_type, total, (fractionals >> 1)> root_type;

        typedef number_type::value_type value_type;

        static number_type const zero(0.0);
        if (x < zero) {
            throw std::exception("sqrt parameter is negative");
        }

        // procedure logics: we search for max integer which square is less than x
        // so for fixed-point square root format is: [n + m/2 | m/2] where
        // n is total bits of input integer and m is its fractional

        // integer to get square root of
        value_type op = x.value();

        value_type res(0);
        value_type one = value_type(1) << (std::numeric_limits<value_type>::digits - 1);

        while (one > op) {
            one >>= 2;
        }

        while (one != 0) {
            if (op >= res + one) {
                op = op - (res + one);
                res = res + (one << 1);
            }

            res >>= 1;
            one >>= 2;
        }

        return number_type::wrap(value_type(res << root_type::fractionals));
    }

    /// stubs
    template<typename storage_type, size_t total, size_t fractionals>
    number<storage_type, total, fractionals> expf(number<storage_type, total, fractionals> const x)
    {
        typedef number<storage_type, total, fractionals> number_type;
        number_type const val(std::exp(static_cast<double>(x)));

        return val;
    }

    template<typename storage_type, size_t total, size_t fractionals>
    number<storage_type, total, fractionals> logf(number<storage_type, total, fractionals> const x)
    {
        typedef number<storage_type, total, fractionals> number_type;
        number_type const val(std::log(static_cast<double>(x)));

        return val;
    }

    /// @brief numeric limits specialization for fixed-point
    template<typename storage_type, size_t total, size_t fractionals>
    class numeric_limits<number<storage_type, total, fractionals> >
    {
        typedef number<storage_type, total, fractionals> number_type;

    public:
        /// @brief does type allow denormalized values
        static const float_denorm_style has_denorm = denorm_absent;

        /// @brief does accuracy loss treats as a denormalization loss rather
        ///        than as an inexact result
        static const bool has_denorm_loss = false;

        static const bool has_infinity = false;
        static const bool has_quiet_NaN = false;
        static const bool has_signaling_NaN = false;

        /// @brief is set of type values finite
        static const bool is_bounded = true;

        /// @brief have rounding errors no impact on calculations
        static const bool is_exact = true;

        /// @brief does type conform to IEC559 standard
        static const bool is_iec599 = false;

        static const bool is_integer = false;
        static const bool is_modulo = false;

        static const bool is_signed = std::numeric_limits<typename number_type::value_type>::is_signed;

        /// @brief is this an explicit specialization of std::numeric_limits for 
        ///        current type
        static const bool is_specialized = true;

        /// @brief could type determine that value is too small not to be normalized
        ///        to zero
        static const bool tinyless_before = false;

        /// @brief has current type exceptions for arithmetics troubles
        static const bool traps = true;

        /// @brief how type rounds a float value to its representation
        static const float_round_style round_style = round_to_nearest;

        static const int digits = total;

        //#define M_LOG10_2 M_LOG10E/M_LOG2E
        static const int digits10 = 0;//static_cast<int>(digits * M_LOG10_2 + 0.5);

        static const int max_exponent = 0;//static_pow<2, total - fractionals>::value;
        static const int max_exponent10 = 0;//static_cast<int>(max_exponent * M_LOG10_2);
        static const int min_exponent = 0;//-static_cast<int>(static_pow<2, fractionals>::value);
        static const int min_exponent10 = 0;//static_cast<int>(min_exponent * M_LOG10_2);
        static const int radix = 0;

        /// @brief computes minimum value for current type
        static number_type min()
        {
            if (std::numeric_limits<storage_type>::is_signed) {
                return -max();
            }

            return number_type::wrap(0);
        }

        /// @brief computes maximum value for current type
        static number_type max()
        {
            boost::uintmax_t val = 0;
            if (total >= fractionals) {
                val = static_pow<2, total>::value - 1;
            }

            return number_type::wrap(val);
        }

        /// @brief computes the smallest difference between two values-neighbors
        static number_type epsilon(){ return number_type::wrap(1); }

        /// @brief computes the maximum rounding error for current type
        static number_type round_error(){ return number_type(0.5); }

        static number_type denorm_min(){ return number_type(0); }
        static number_type infinity(){ return number_type(0); }
        static number_type quiet_NaN(){ return number_type(0); }
        static number_type signaling_NaN(){ return number_type(0); }
    };
}

#define _tmpl_head_ template<typename storage_type, size_t n, size_t f>
#define _cls_name_ number<storage_type, n, f>

#include "./details/normalization.inl"

#include "./details/ordering.inl"

#include "./details/arithmetics.inl"

#endif
