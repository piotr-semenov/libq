/// @brief provides std::fmod stuff in case of fixed-point numbers

namespace std {
    /// @brief computes fixed-point remainder of x/y
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> fmod(core::fixed_point<T, n, f, op, up> const& a,
        core::fixed_point<T, n, f, op, up> const& b)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        static T const mask(boost::low_bits_mask_t<f>::sig_bits);

        fp const fractional = fp::wrap(fp(a / b).value() & mask);
        return fp(fractional * b);
    }
}
