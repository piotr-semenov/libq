/// @brief provides CORDIC for tanh function

namespace std {
    /// @brief computes tanh as ratio of sinh and cosh
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> tanh(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> type;

        return type(std::sinh(val) / std::cosh(val));
    }
}
