/// @brief provides fixed-point quotient info

#ifndef INC_QUOTIENT_INFO_HPP_
#define INC_QUOTIENT_INFO_HPP_

#include "./../Common/number.hpp"

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

    template<typename T, size_t total, size_t fractionals>
    class number;

    /// @brief meta-class to inference computation primitive quotient type.
    ///        In case of float-point built-in types
    template<typename value_type>
    class quotient_info
    {
        BOOST_STATIC_ASSERT(boost::is_float<value_type>::value);

    public:
        typedef value_type quotient_type;                ///< if type is floating then division is closed operation
        typedef value_type quotient_value_type;

        /// @brief sign meta-property of type value_type
        struct sign_info
        {
            enum { value = true };
        };

        /// @brief existence of arithmetic operations result for current type
        struct closing_info
        {
            enum { value = true };
        };
    };

    /// @brief in case of fixed-point types
    template<typename T, size_t total, size_t fractionals>
    class quotient_info<number<T, total, fractionals> >
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
            enum { value = !(total + fractionals <= std::numeric_limits<max_type>::digits) };
        };

        // logics: 1. if we don't have integral type with enough bits amount then we 
        //            interpret division as closed operation
        //         2. boost::int_t template parameter takes into account a sign bit
        /// @brief integral value type for fixed-point result type of division
        struct value_type_info
        {
            /// @brief in case if quotient type is non-closed under arithmetic operations
            struct op
            {
                typedef typename if_<sign_info, typename boost::int_t<sign_info::sign_bit + total + fractionals>::least,
                    typename boost::uint_t<total + fractionals>::least>::type type;
            };

            /// @brief in case if quotient type is closed under arithmetic operations
            struct cl
            {
                typedef T type;
            };
        };

        ///< integral type for quotient value type
        typedef typename eval_if<closing_info, typename value_type_info::cl,
                                               typename value_type_info::op>::type quotient_value_type;

        ///< fixed-point type for quotient result
        typedef typename if_<closing_info, operand_type,
                                           number<quotient_value_type, total + fractionals, 2 * fractionals> >::type quotient_type;
    };
}

#endif
