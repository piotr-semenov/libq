/// @brief provides info class about subtraction result type

#ifndef INC_CORE_DIFF_OF_HPP_
#define INC_CORE_DIFF_OF_HPP_

namespace libq {
    template<typename word_type>
    class sum_of;

    /// @brief tool for type inference of the subtraction result with fixed-point
    /// and floating-point numbers
    template<typename word_type>
    class diff_of
        :    public sum_of<word_type>
    {};
}

#endif
