/// @brief provides std::round stuff in case of fixed-point numbers

namespace std {
    /// @brief std::round in case of fixed-point number
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> round(core::fixed_point<T, n, f, op, up> const& x)
    {
        typedef core::fixed_point<T, n, f, op, up> type;
        static T const mask(boost::low_bits_mask_t<f>::least);

        type::word_type val = x.value() + (x > type(0)) ? type(0.5).value() :
            type(-0.5).value();

        return type::wrap(val & mask);
    }
}
