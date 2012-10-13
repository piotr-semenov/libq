/// @brief provides fixed-point sum info

#ifndef INC_SUM_INFO_HPP_
#define INC_SUM_INFO_HPP_

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

    template<typename storage_type, size_t total, size_t fractionals>
    class number;

    /// @brief tool for type inference of the summ result with fixed-point and
    /// floating-point numbers
    template<typename value_type>
    class sum_info
    {
    public:
        typedef value_type sum_type;
        typedef value_type sum_value_type;

        struct sign_info
        {
            enum { value = true };
        };

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

    /// @brief in case of fixed-point numbers
    template<typename T, size_t n, size_t f>
    class sum_info<number<T, n, f> >
    {
        typedef number<T, n, f> operand_type;

    public:
        ///< is the type of summ a signed type?
        struct is_signed
        {
            enum { value = std::numeric_limits<T>::is_signed };
        };

        ///< is type of the summ closed under arithmetic operations?
        struct is_closed
        {
            typedef typename if_<is_signed, boost::intmax_t, boost::uintmax_t>::type
                max_type;

            // total bits and std::numeric_limits do not count sign bit
            enum { value = !(n + 1 <= std::numeric_limits<max_type>::digits) };
        };

        // brief: 1. if one do not have integral type of enough bits count then
        //           it has to interpret summation as a closed operation;
        //        2. boost::int_t template parameter takes into account a sign bit.
        ///< integral value type below fixed-point number as a summ result type
        struct value_type_info
        {
            ///< in case if summ type is not closed under arithmetic operations
            struct op
            {
                typedef typename if_<is_signed, typename boost::int_t<is_signed::value + n + 1u>::least,
                    typename boost::uint_t<n + 1u>::least>::type type;
            };

            /// @brief in case if sum type is closed under arithmetic operations
            struct cl
            {
                typedef T type;
            };
        };

        ///< integral value type that is below of fixed-point result type of the summ
        typedef typename eval_if<is_closed, typename value_type_info::cl,
            typename value_type_info::op>::type sum_value_type;

        ///< fixed-point type for summ result
        typedef typename if_<is_closed, operand_type, number<sum_value_type, n + 1, f> >::type
            sum_type;
    };
}

#endif
