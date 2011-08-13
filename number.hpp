/// @mainpage fixed-point stuff

#ifndef INC_FIXED_POINT_NUMBER_HPP_
#define INC_FIXED_POINT_NUMBER_HPP_

#include "./../Common/quotient_info.hpp"
#include "./../Common/product_info.hpp"

#include "./../Common/diff_info.hpp"
#include "./../Common/sum_info.hpp"

#include "./../Common/static_pow.hpp"

#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>

#include <boost/integer.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/cstdint.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/make_signed.hpp>

#include <boost/integer_traits.hpp>

#include <boost/operators.hpp>

#include <limits>
#include <cmath>

/// namespace utils
/// @brief fixed-point numbers stuff
/// @detailed this contains fixed-point number, product type inference by template techniques,
///            CORDIC implementation of all wide-used elementary functions, utils for flexible
///            substitution of floating-point with fixed-point and vice versa
namespace utils {
    namespace {
        namespace mpl = boost::mpl;
    }

    /// @brief fixed-point number
    /// @detailed class should be parameterized by integral type: no checks will be made for point position,
    ///           it can be any non-negative integer
    /// @param storage_type built-in integral type that represents fixed-point number
    /// @param n total bits count for fixed-point logic (without sign bit if it need).
    ///          So if storage_type is signed then total bits representing type is (n + 1).
    ///          If storage_type is unsigned then total bits representing type is n exactly.
    /// @param f count of fractional part bits for fixed-point logic
    template<typename storage_type, size_t n, size_t f>
    class number
        :    public boost::totally_ordered<number<storage_type, n, f> >,   // >, <=, >=, != in terms of <, == and !
             public boost::shiftable<number<storage_type, n, f>, size_t>
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<storage_type>));

        BOOST_STATIC_ASSERT((f >= 0));
        BOOST_STATIC_ASSERT((n <= std::numeric_limits<storage_type>::digits));

        typedef number<storage_type, n, f> this_class;

    public:
        typedef this_class type;

        typedef storage_type value_type;                                        ///< integral type to model fixed-point logic
        static size_t const fractionals = f;                                    ///< count of fractional part bits
        static size_t const total = n;                                          ///< count of whole part bits
        static boost::uintmax_t const scale = static_pow<2, fractionals>::value;  ///< scale to convert from float to fixed-point and vise versa

        static size_t const sign_bit = (std::numeric_limits<value_type>::is_signed) ? 1u : 0; ///< fixed-point number sign bit

        /// @brief integral bounds for fixed-point number values
        /// @detailed fixed-point number is an modeled by an built-in integer.
        ///           If fixed-point format does not fit an integer dynamic range,
        ///           we have to check its user-defined intrinsic range.
        class bounds
        {
        public:
            /// @brief gets range highest value
            // logics: we want to get from (2^n - 1) a value (2^m - 1), where n is digits count for extended type
            //         and m is digits count for fixed-point format
            //         we could do it in next way: (2^n - 1)/2^(n-m) = 2^m - 1/2^(n-m)
            //         term -1/2^(n-m) will be truncated to just to -1 during right shift
            static value_type const max = (boost::integer_traits<value_type>::const_max >> (boost::integer_traits<value_type>::digits - total));

        private:
            // in case of signed type
            template<bool t>
            struct min_bound_traits
            {
                static value_type const value = -max - 1;
            };

            // in case of unsigned types
            template<>
            struct min_bound_traits<false>
            {
                static value_type const value = value_type(0);
            };

        public:
            /// @brief gets range lowest value
            static value_type const min = min_bound_traits<sign_bit>::value;

            /// @brief cast from any integer type T to internal integer type
            /// @detailed cast is valid if value could be fit with dynamic range for current
            ///           fixed-point format
            /// @throw boost::numeric::positive_overflow or boost::numeric::negative_overflow
            ///        if value is out of range
            template<typename T>
            static value_type internal_cast(T val);
        };

        explicit number()
            :    m_value(0){}

        /// @brief converts from any arithmetic value
        template<typename T>
        explicit number(T value)
            :    m_value(convert_of(value)){}

        /// @brief copy constructor for same-format fixed-point object
        explicit number(this_class const& x)
            :    m_value(x.value()){}

        /// @brief copy constructor for another-format fixed-point object
        template<typename storage_type1, size_t total1, size_t fractionals1>
        number(number<storage_type1, total1, fractionals1> const& x)
            :    m_value(normalize_of<storage_type1, fractionals1>(x.value())){}

        /// @brief cast to numeric type
        template<typename T>
        operator T() const
        {
            BOOST_STATIC_ASSERT((boost::is_arithmetic<T>::value));

            return boost::numeric_cast<T>(to_float()); 
        }

        /// @brief copy operator for floating values
        template<typename T>
        void operator =(T const x)
        {
            BOOST_STATIC_ASSERT((boost::is_arithmetic<T>::value));

            this->m_value = convert_of(x); 
        }

        /// @brief copy operator for fixed-point of same format
        this_class& operator =(this_class const& x)
        {
            this->m_value = x.value();
            return *this;
        }

        /// @brief copy operator for fixed-point of another format
        template<typename storage_type1, size_t total1, size_t fractionals1>
        this_class& operator =(number<storage_type1, total1, fractionals1> const& x)
        {
            this->m_value = normalize_of<storage_type1, fractionals1>(x.value());
            return *this;
        }

        void value(value_type const x){ this->m_value = x; }
        value_type const& value() const{ return this->m_value; }

        /// @brief converts any arithmetic type to fixed-point integral representation
        template<typename T>
        static value_type convert_of(T const x)
        {
            BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value);

            value_type const val = boost::numeric_cast<value_type>(x * static_cast<value_type>(this_class::scale));
            return val;
        }

        /// @brief normalizes specified fixed-point format integer to current format
        template<typename storage_type1, size_t fractionals1>
        static value_type normalize_of(storage_type1 const x)
        {
            BOOST_CONCEPT_ASSERT((boost::IntegerConcept<storage_type1>));

            static size_t const shifts = (fractionals > fractionals1) ? (fractionals - fractionals1) : (fractionals1 - fractionals);
            static size_t const max_bits = std::numeric_limits<boost::uintmax_t>::digits;
            typedef boost::uintmax_t maxint_type;

            //if (std::numeric_limits<value_type>::is_signed) {
            //    max_bits = std::numeric_limits<boost::intmax_t>::digits;
            //    typedef boost::intmax_t maxint_type;
            //}
            //else {
            //    max_bits = std::numeric_limits<boost::uintmax_t>::digits;
            //}

            value_type val(0);
            value_type const converted = boost::numeric_cast<value_type>(x);

            using boost::mpl::eval_if_c;
            if (fractionals > fractionals1) {
                struct t1
                {
                    typedef boost::int_t<total + shifts>::least type;
                };

                struct t2
                {
                    typedef maxint_type type;
                };

                typedef eval_if_c<total + shifts < max_bits, t1, t2>::type extended_type;
                extended_type t = static_cast<extended_type>(x);
                t <<= shifts;

                //bounds::check(t);
                val = static_cast<value_type>(t);
            }
            else {
//                bounds::check(x >> shifts);
                val = static_cast<value_type>(x >> shifts);
            }

            return val;
        }

        /// @brief converts fixed-point to float
        double to_float() const
        {
            double const val = static_cast<double>(value()) / this_class::scale;
            return val;
        }

        /// @brief combines specified integral value to fixed-point instance
        static this_class create(value_type const val)
        {
            this_class x;
            x.m_value = val;

            return x;
        }

        typedef typename boost::make_signed<value_type>::type signed_value_type;
        static number<signed_value_type, total, fractionals> make_signed(this_class const& x)
        {
            return number<signed_value_type, total, fractionals>(x);
        }

        ///////////////////////////// ORDERING OPERATORS /////////////////////////////////////
        /// @brief binary relations for fixed-point numbers
        bool operator <(this_class const& x) const{ return value() < x.value(); }
        bool operator ==(this_class const& x) const{ return value() == x.value(); }
        bool operator !() const{ return value() == 0; }

        //////////////////////////// +,-,*,/-OPERATORS ///////////////////////////////////////
        typedef typename sum_info<this_class>::sum_type sum_type;
        typedef typename diff_info<this_class>::diff_type diff_type;
        typedef typename product_info<this_class>::product_type product_type;
        typedef typename quotient_info<this_class>::quotient_type quotient_type;

    private:
        static boost::intmax_t const mod = static_pow<2, total - 1>::value;

        // if addition is non-closed operation: integral type to represent sum is existed
        template<typename Other_type>
        sum_type add(this_class const& a, Other_type const& b, mpl::bool_<true>::type) const
        {
            typedef sum_type::value_type sum_value_type;

            sum_value_type const val = boost::numeric_cast<sum_value_type>(a.value()) + this_class(b).value();
            return sum_type::create(val);
        }

        // if addition is closed operation: integral type to represent sum isn't existed
        template<typename Other_type>
        this_class add(this_class const& a, Other_type const& b, mpl::bool_<false>::type) const
        {
            value_type const sum = a.value() + this_class(b).value();

            value_type const val = static_cast<value_type>(sum % this_class::mod);
            return this_class::create(val);
        }

        // if subtraction is non-closed operation
        template<typename Other_type>
        diff_type subtract(this_class const& a, Other_type const& b, mpl::bool_<true>::type) const
        {
            typedef diff_type::value_type diff_value_type;

            diff_value_type const val = boost::numeric_cast<diff_value_type>(a.value()) - this_class(b).value();
            return diff_type::create(val);
        }

        // if subtraction is closed operation
        template<typename Other_type>
        this_class subtract(this_class const& a, Other_type const& b, mpl::bool_<false>::type) const
        {
            value_type const diff = a.value() - this_class(b).value();

            value_type const val = static_cast<value_type>(diff % this_class::mod);
            return this_class::create(val);
        }

        // if product is non-closed operation
        template<typename Other_type>
        product_type product(this_class const& a, Other_type const& b, mpl::bool_<true>::type) const
        {
            typedef product_type::value_type product_value_type;

            product_value_type const val = boost::numeric_cast<product_value_type>(a.value()) * this_class(b).value();
            return product_type::create(val);
        }

        // if product is closed operation: case of integral value
        template<typename Other_type>
        this_class product(this_class const& a, Other_type const& b, mpl::bool_<false>::type) const
        {
            BOOST_CONCEPT_ASSERT((boost::IntegerConcept<Other_type>));

            this_class x = a;
            x *= b;

            return x;
        }

        /// !!!! here is a bottleneck in fixed-point implementation performance
        // if product is closed operation: case of another fixed-point
        template<typename storage_type1, size_t total1, size_t fractionals1>
        this_class product(this_class const& a, number<storage_type1, total1, fractionals1> const& b, mpl::bool_<false>::type) const
        {
            this_class x;

            value_type const val = a.value() * static_cast<value_type>(b.value());
            x.value(val >> fractionals1);

            return x;
        }

        // if division is non-closed operation
        template<typename Other_type>
        quotient_type divide(this_class const& a, Other_type const &b, mpl::bool_<true>::type) const
        {
            typedef quotient_type::value_type quotient_value_type;

            quotient_value_type const shifted = (boost::numeric_cast<quotient_value_type>(a.value()) << fractionals);
            quotient_value_type const converted = this_class(b).value();
            quotient_value_type const val = shifted / converted;

            return quotient_type::create(val << fractionals);
        }

        // if division is closed operation: case of integral value
        template<typename Other_type>
        this_class divide(this_class const& a, Other_type const &b, mpl::bool_<false>::type) const
        {
            BOOST_CONCEPT_ASSERT((boost::IntegerConcept<Other_type>));

            this_class x = a;
            x *= b;

            return x;
        }

        // if division is closed operation: case of another fixed-point
        template<typename storage_type1, size_t total1, size_t fractionals1>
        this_class divide(this_class const& a, number<storage_type1, total1, fractionals1> const&b, mpl::bool_<false>::type) const
        {
            this_class x;

            // we have no additional precision for quotient due to operation closeness
            value_type const val = (static_cast<value_type>(1u) << fractionals1) / static_cast<value_type>(b.value());
            x.value(a.value() * val);

            return x;
        }

    public:

        /// @brief adds a number that will be converting to fixed-point form firstly
        template<typename Other_type>
        sum_type operator +(Other_type const& x) const
        { 
            sum_type const val = add(*this, x, mpl::bool_<!(sum_info<this_class>::closing_info::value)>::type());
            return val;
        }

        /// @brief subtracts a number that will be converting to fixed-point form firstly
        template<typename Other_type>
        diff_type operator -(Other_type const& x) const
        {
            diff_type const val = subtract(*this, x, mpl::bool_<!(diff_info<this_class>::closing_info::value)>::type());
            return val;
        }

        /// @brief multiplies by a number that will be converting to fixed-point form firstly
        template<typename Other_type>
        product_type operator *(Other_type const& x) const
        {
            product_type const val = product(*this, x, mpl::bool_<!(product_info<this_class>::closing_info::value)>::type());
            return val;
        }

        /// @brief divides by a number that will be converting to fixed-point form firstly
        template<typename Other_type>
        quotient_type operator /(Other_type const& x) const
        {
            quotient_type const val = divide(*this, x, mpl::bool_<!(quotient_info<this_class>::closing_info::value)>::type());
            return val;
        }

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
        this_class operator -() const{ return this_class::create(-value()); }

    private:
        value_type m_value;
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

        return number_type::create(val);
    }

    /// @brief computes fixed-point remainder of x/y
    template<typename storage_type, size_t total, size_t fractionals>
    number<storage_type, total, fractionals> fmod(number<storage_type, total, fractionals> const& a,
                                                  number<storage_type, total, fractionals> const& b)
    {
        return number<storage_type, total, fractionals>::create(a.value() % b.value());
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

        return number_type::create(value_type(res << root_type::fractionals));
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

            return number_type::create(0);
        }

        /// @brief computes maximum value for current type
        static number_type max()
        {
            boost::uintmax_t val = 0;
            if (total >= fractionals) {
                val = static_pow<2, total>::value - 1;
            }

            return number_type::create(val);
        }

        /// @brief computes the smallest difference between two values-neighbors
        static number_type epsilon(){ return number_type::create(1); }

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

#include "./../Common/details/bounds.inl"

#endif
