/// @brief provides std::round stuff in case of fixed-point numbers

namespace std {
    /// @brief rounds
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> round(utils::number<T, n, f, op, up> const& x)
    {
        typedef utils::number<T, n, f, op, up> type;
        static T const mask(boost::low_bits_mask_t<f>::least);

        type::value_type val = x.value() + (x > type(0)) ? type(0.5).value() :
            type(-0.5).value();

        return type::wrap(val & mask);
    }
}
