/// @brief provides CORDIC for arcsinh function

namespace std {
    /// @brief computes arcinh as logarithm
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> asinh(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> type;

        type const arg(val + std::sqrt(type(val * val + 1)));
        type x = type(std::log(arg) / type::CONST_LOG2E);

        return x;
    }
}
