/// @brief provides std::ceil stuff in case of fixed-point numbers

namespace std {
    /// @brief std::ceil in case of fixed-point numbers
    template<typename T, size_t n, size_t f, class op, class up>
    libq::fixed_point<T, n, f, op, up> ceil(libq::fixed_point<T, n, f, op, up> const& x)
    {
        typedef libq::fixed_point<T, n, f, op, up> type;
        static T const mask(boost::low_bits_mask_t<f>::least);

        type::word_type val = x.value();
        if ((val & mask) != 0) {
            val += T(1u) << f;
        }

        return type::wrap(val);
    }
}
