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
    {};
}

#endif
