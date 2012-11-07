/// @brief provides std::fmod stuff in case of fixed-point numbers

namespace std {
    /// @brief computes fixed-point remainder of x/y
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> fmod(utils::number<T, n, f, op, up> const& a,
        utils::number<T, n, f, op, up> const& b)
    {
        typedef utils::number<T, n, f, op, up> fixed_point;
        static T const mask(boost::low_bits_mask_t<f>::sig_bits);

        fixed_point const fractional = fixed_point::wrap(
            fixed_point(a / b).value() & mask
        );
        return fixed_point(fractional * b);
    }
}
