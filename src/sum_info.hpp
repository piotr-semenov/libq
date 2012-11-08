/// @brief provides info class about summ result type

#ifndef INC_SUM_INFO_HPP_
#define INC_SUM_INFO_HPP_

#include <boost/integer.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>

#include <limits>
#include <boost/cstdint.hpp>

namespace core {
    namespace {
        using boost::mpl::if_;
        using boost::mpl::eval_if;
    }

    template<typename T, size_t n, size_t f, class op, class up>
    class fixed_point;

    /// @brief tool for type inference of the summ result with fixed-point and
    /// floating-point numbers
    template<typename word_type>
    class sum_info
    {
    public:
        typedef word_type sum_type;
        typedef word_type sum_word_type;

        struct sign_info
        {
            enum { value = true };
        };

        struct closing_info
        {
            enum { value = true };
        };

        struct word_type_info
        {
            struct op
            {
                typedef word_type type;
            };

            struct cl
            {
                typedef word_type type;
            };
        };
    };

    /// @brief in case of fixed-point numbers
    template<typename T, size_t n, size_t f, class op, class up>
    class sum_info<fixed_point<T, n, f, op, up> >
    {
        typedef fixed_point<T, n, f, op, up> operand_type;

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
            enum { value = !(n + 1u + is_signed::value <= std::numeric_limits<max_type>::digits) };
        };

        // brief: 1. if one do not have integral type of enough bits count then
        //           it has to interpret summation as a closed operation;
        //        2. boost::int_t template parameter takes into account a sign bit.
        ///< integral value type below fixed-point number as a summ result type
        struct word_type_info
        {
            ///< in case if summ type is not closed under arithmetic operations
            struct op
            {
                // Bits includes sign bit in boost::int_t<Bits> template
                typedef typename if_<is_signed, typename boost::int_t<n + 2u>::least,
                    typename boost::uint_t<n + 1u>::least>::type type;
            };

            /// @brief in case if sum type is closed under arithmetic operations
            struct cl
            {
                typedef T type;
            };
        };

        ///< integral value type that is below of fixed-point result type of the summ
        typedef typename eval_if<is_closed, typename word_type_info::cl,
            typename word_type_info::op>::type sum_word_type;

        ///< fixed-point type for summ result
        typedef typename if_<is_closed, operand_type, fixed_point<sum_word_type, n + 1u,
            f, op, up> >::type sum_type;
    };
}

#endif
