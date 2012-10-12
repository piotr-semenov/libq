/// @brief provides fixed-point sum info

#ifndef INC_SUM_INFO_HPP_
#define INC_SUM_INFO_HPP_

#include "./number.hpp"

#include <boost/integer.hpp>
#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>

#include <limits>
#include <boost/cstdint.hpp>

namespace utils {
    namespace {
        using boost::mpl::if_;
        using boost::mpl::eval_if;
    }

    // fixed-point number class
    template<typename T, size_t total, size_t fractionals>
    class number;

    /// @brief meta-class to inference fixed-point sum type.
    ///        In case of float-point built-in types
    template<typename value_type>
    class sum_info
    {
        BOOST_STATIC_ASSERT((boost::is_float<value_type>::value));

    public:
        typedef value_type sum_type;                ///< if type is floating then summation is closed operation
        typedef value_type sum_value_type;

        /// @brief is sum type signed?
        struct sign_info
        {
            enum { value = true };
        };

        /// @brief is sum type closed under arithmetic operations
        struct closing_info
        {
            enum { value = true };
        };

        struct value_type_info
        {
            struct op
            {
                typedef value_type type;
            };

            struct cl
            {
                typedef value_type type;
            };
        };
    };

    /// @brief in case of fixed-point type
    template<typename T, size_t total, size_t fractionals>
    class sum_info<number<T, total, fractionals> >
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));

        typedef number<T, total, fractionals> operand_type;

    public:
        /// @brief sign meta-property of type value_type
        struct sign_info
        {
            enum { value = std::numeric_limits<T>::is_signed };
            static size_t const sign_bit = 1u;          // only to increase code readability
        };

        /// @brief existence of arithmetic operations result for current type
        struct closing_info
        {
            typedef typename if_<sign_info, boost::intmax_t, boost::uintmax_t>::type max_type;

            // total bits and std::numeric_limits don't count sign bit
            // if current condition holds then type is closed over arithmetic
            // operations
            enum { value = !(total + 1 <= std::numeric_limits<max_type>::digits) };
        };

        // logics: 1. if we don't have integral type with enough bits amount then we 
        //            interpret summation as closed operation
        //         2. boost::int_t template parameter takes into account a sign bit
        /// @brief integral value type for fixed-point result type of sum
        struct value_type_info
        {
            /// @brief in case if sum type is non-closed under arithmetic operations
            struct op
            {
                typedef typename if_<sign_info, typename boost::int_t<sign_info::sign_bit + total + 1>::least,
                    typename boost::uint_t<total + 1>::least>::type type;
            };

            /// @brief in case if sum type is closed under arithmetic operations
            struct cl
            {
                typedef T type;
            };
        };

        ///< integral value type for fixed-point result type of sum
        typedef typename eval_if<closing_info, typename value_type_info::cl,
                                               typename value_type_info::op>::type sum_value_type;

        ///< fixed-point type for sum result
        typedef typename if_<closing_info, operand_type,
                                           number<sum_value_type, total + 1, fractionals> >::type sum_type;
    };
}

#endif
