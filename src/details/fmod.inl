/// @brief provides std::fmod stuff in case of fixed-point numbers

namespace std {
    /// @brief std::fmod computes fixed-point remainder of double(x)/double(y)
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> fmod(core::fixed_point<T, n, f, op, up> const& a,
        core::fixed_point<T, n, f, op, up> const& b)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        typedef fp::word_type word_type;

        word_type const quotient = word_type(a.value()) / word_type(b.value());
        word_type remainder = word_type(a.value()) - word_type(quotient * b.value());
        if (remainder < 0) {
            remainder += word_type(b.value());
        }

        return fp::wrap(remainder);
    }
}
