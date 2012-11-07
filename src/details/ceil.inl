/// @brief provides std::ceil stuff in case of fixed-point numbers

namespace std {
    /// @brief ceil
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> ceil(utils::number<T, n, f, op, up> const& x)
    {
        typedef utils::number<T, n, f, op, up> type;
        static T const mask(boost::low_bits_mask_t<f>::least);

        type::value_type val = x.value();
        if ((val & mask) != 0) {
            val += T(1u) << f;
        }

        return type::wrap(val);
    }
}
