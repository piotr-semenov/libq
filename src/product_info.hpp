/// @brief provides info class for product result type

#ifndef INC_PRODUCT_INFO_HPP_
#define INC_PRODUCT_INFO_HPP_

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

    template<typename storage_type, size_t total, size_t fractionals, class op,
    class up>
    class number;

    /// @brief tool for type inference of the product result with fixed-point and
    /// floating-point numbers
    template<typename value_type>
    class product_info
    {
    public:
        typedef value_type product_type;
        typedef value_type product_value_type;

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
    template<typename T, size_t n, size_t f, class op, class up>
    class product_info<number<T, n, f, op, up> >
    {
        typedef number<T, n, f, op, up> operand_type;

    public:
        ///< is the type of product a signed type?
        struct is_signed
        {
            enum { value = std::numeric_limits<T>::is_signed };
        };

        ///< is type of the product closed under arithmetic operations?
        struct is_closed
        {
            typedef typename if_<is_signed, boost::intmax_t, boost::uintmax_t>::type max_type;

            // total bits and std::numeric_limits do not count sign bit
            enum { value = !(2u * n + is_signed::value <= std::numeric_limits<max_type>::digits) };
        };

        // brief: 1. if one do not have integral type of enough bits count then
        //           it has to interpret multiplication as a closed operation;
        //        2. boost::int_t template parameter takes into account a sign bit.
        ///< integral value type below fixed-point number as a product result type
        struct value_type_info
        {
            ///< in case if product type is not closed under arithmetic operations
            struct op
            {
                typedef typename if_<is_signed, typename boost::int_t<2u * n + 1u>::least,
                    typename boost::uint_t<2u * n>::least>::type type;
            };

            /// @brief in case if product type is closed under arithmetic operations
            struct cl
            {
                typedef T type;
            };
        };

        ///< integral value type that is below of fixed-point result type of the
        /// multiplication
        typedef typename eval_if<is_closed, typename value_type_info::cl,
            typename value_type_info::op>::type product_value_type;

        ///< fixed-point type for product result
        typedef typename if_<is_closed, operand_type,
            number<product_value_type, 2u * n, 2u * f, op, up> >::type product_type;
    };
}

#endif
