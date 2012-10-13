/// @brief provides fixed-point difference info

#ifndef INC_DIFFERENCE_INFO_HPP_
#define INC_DIFFERENCE_INFO_HPP_

namespace utils {
    template<typename value_type>
    class sum_info;

    /// @brief tool for type inference of the subtraction result with fixed-point
    /// and floating-point numbers
    template<typename value_type>
    class diff_info
        :    public sum_info<value_type>
    {
    public:
        ///< integral value type that is below of fixed-point result type of the
        /// subtraction (or just a floating-point number)
        typedef typename sum_info<value_type>::sum_value_type diff_value_type;

        ///< type for subtraction result
        typedef typename sum_info<value_type>::sum_type diff_type;
    };
}

#endif
