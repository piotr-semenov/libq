/// @brief provides info class about subtraction result type

#ifndef INC_CORE_DIFF_INFO_HPP_
#define INC_CORE_DIFF_INFO_HPP_

namespace core {
    template<typename word_type>
    class sum_info;

    /// @brief tool for type inference of the subtraction result with fixed-point
    /// and floating-point numbers
    template<typename word_type>
    class diff_info
        :    public sum_info<word_type>
    {
    public:
        ///< integral value type that is below of fixed-point result type of the
        /// subtraction (or just a floating-point number)
        typedef typename sum_info<word_type>::sum_word_type diff_word_type;

        ///< type for subtraction result
        typedef typename sum_info<word_type>::sum_type diff_type;
    };
}

#endif
