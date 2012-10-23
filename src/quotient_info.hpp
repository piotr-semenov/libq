/// @brief provides info class for division operation result type

#ifndef INC_QUOTIENT_INFO_HPP_
#define INC_QUOTIENT_INFO_HPP_

#include <boost/integer.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>

#include <limits>
#include <boost/cstdint.hpp>

namespace utils {
    namespace {
        using boost::mpl::if_;
        using boost::mpl::eval_if;
    }

    template<typename storage_type, size_t total, size_t fractionals,
    class OP = overflow_policy::just_do, class UP = underflow_policy::just_do>
    class number;

    /// @brief tool for type inference of the division result with fixed-point and
    /// floating-point numbers
    template<typename value_type>
    class quotient_info
    {
    public:
        typedef value_type quotient_type;
        typedef value_type quotient_value_type;

        struct is_signed
        {
            enum { value = true };
        };

        struct is_closed
        {
            enum { value = true };
        };
    };

    /// @brief in case of fixed-point numbers
    template<typename T, size_t n, size_t f, class OP, class UP>
    class quotient_info<number<T, n, f, OP, UP> >
    {
        typedef number<T, n, f, OP, UP> operand_type;

    public:
        ///< is the type of division a signed type?
        struct is_signed
        {
            enum { value = std::numeric_limits<T>::is_signed };
        };

        ///< is type of the division closed under arithmetic operations?
        struct is_closed
        {
            typedef typename if_<is_signed, boost::intmax_t, boost::uintmax_t>::type max_type;

            // total bits and std::numeric_limits do not count sign bit
            enum { value = !(n + f <= std::numeric_limits<max_type>::digits) };
        };

        // logics: 1. if we don't have integral type with enough bits amount then we 
        //            interpret division as closed operation
        //         2. boost::int_t template parameter takes into account a sign bit
        /// @brief integral value type for fixed-point result type of division
        struct value_type_info
        {
            ///< in case if quotient type is not closed under arithmetic operations
            struct op
            {
                typedef typename if_<is_signed, typename boost::int_t<is_signed::value + n + f>::least,
                    typename boost::uint_t<n + f>::least>::type type;
            };

            /// @brief in case if quotient type is closed under arithmetic operations
            struct cl
            {
                typedef T type;
            };
        };

        ///< integral value type that is below of fixed-point result type of the
        /// division
        typedef typename eval_if<is_closed, typename value_type_info::cl,
            typename value_type_info::op>::type quotient_value_type;

        ///< fixed-point type for division result
        typedef typename if_<is_closed, operand_type,
            number<quotient_value_type, n + f, 2u * f, OP, UP> >::type quotient_type;
    };
}

#endif
