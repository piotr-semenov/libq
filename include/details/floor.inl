/// @brief provides std::floor stuff in case of fixed-point numbers

namespace std {
    /// @brief std::floor in case of fixed-point numbers
    template<typename T, size_t n, size_t f, class op, class up>
    libq::fixed_point<T, n, f, op, up> floor(libq::fixed_point<T, n, f, op, up> const& x)
    {
        typedef libq::fixed_point<T, n, f, op, up> type;
        static T const mask(boost::high_bits_mask_t<std::numeric_limits<T>::digits -
            f>::least);

        return type::wrap(x.value() & mask);
    }
}
