/// @brief provides fixed-point difference info

#ifndef INC_DIFFERENCE_INFO_HPP_
#define INC_DIFFERENCE_INFO_HPP_

#include "./../Common/sum_info.hpp"

namespace utils {
    /// @brief meta-class to inference computation primitive subtraction type
    template<typename value_type>
    class diff_info
    {
    public:
        /// @brief sign meta-property of type value_type
        struct sign_info
        {
            enum { value = sum_info<value_type>::sign_info::value };
        };

        /// @brief existence of arithmetic operations result for current type
        struct closing_info
        {
            enum { value = sum_info<value_type>::closing_info::value };
        };

        /// @brief integral value type for fixed-point result type of sum
        struct value_type_info
        {
            /// @brief in case if sum type is non-closed under arithmetic operations
            struct op
            {
                typedef typename sum_info<value_type>::value_type_info::op::type type;
            };

            /// @brief in case if sum type is closed under arithmetic operations
            struct cl
            {
                typedef typename sum_info<value_type>::value_type_info::cl::type type;
            };
        };

        typedef typename sum_info<value_type>::sum_type diff_type;              ///< type of difference (in case non-POD-types)
        typedef typename sum_info<value_type>::sum_value_type diff_value_type;  ///< type of difference value
    };
}

#endif
