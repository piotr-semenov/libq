/// @brief provides CORDIC for sinh function

namespace std {
    /// @brief computes sinh as sum of exponents
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> sinh(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> type;

        type const arg(val * type::CONST_LOG2E);
        type x = type(std::exp(arg) - std::exp(-arg));
        as_native(x) >>= 1u;

        return x;
    }
}
