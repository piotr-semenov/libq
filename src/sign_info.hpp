/// @brief provides info class about sign of arithmetic operations result type

#ifndef INC_SIGN_INFO_HPP_
#define INC_SIGN_INFO_HPP_

#include <boost/integer.hpp>

#include <limits>

namespace utils {
    template<typename storage_type, size_t total, size_t fractionals,
    class OP = overflow_policy::just_do, class UP = underflow_policy::just_do>
    class number;

    /// @brief tool for sign inference of the result in case of arithmetical
    /// operations with fixed-point and floating-point numbers
    template<typename T, typename U>
    class sign_info
    {
    public:
        enum { sign = true };
    };

    /// @brief in case of fixed-point numbers
    template<typename T1, size_t n1, size_t f1, class OP1, class UP1,
             typename T2, size_t n2, size_t f2, class OP2, class UP2>
    class sign_info<number<T1, n1, f1, OP1, UP1>, number<T2, n2, f2, OP2, UP2> >
    {
        typedef number<T1, n1, f1, OP1, UP1> operand_type1;
        typedef number<T2, n2, f2, OP2, UP2> operand_type2;

    public:
        enum { sign = std::numeric_limits<T1>::is_signed || std::numeric_limits<T2>::is_signed };
    };
}

#endif
