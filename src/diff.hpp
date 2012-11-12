/// @brief provides info class about subtraction result type

#ifndef INC_CORE_DIFF_HPP_
#define INC_CORE_DIFF_HPP_

namespace core {
    template<typename word_type>
    class sum;

    /// @brief tool for type inference of the subtraction result with fixed-point
    /// and floating-point numbers
    template<typename word_type>
    class diff
        :    public sum<word_type>
    {
        template<typename T, size_t n, size_t f, class op, class up>
        friend class fixed_point;

    public:
        ///< integral value type that is below of fixed-point result type of the
        /// subtraction (or just a floating-point number)
        typedef typename sum<word_type>::word_type word_type;

        ///< type for subtraction result
        typedef typename sum<word_type>::type type;
    };
}

#endif
