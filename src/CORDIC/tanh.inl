/// @brief provides CORDIC for tanh function

namespace std {
    /// @brief computes tanh as ratio of sinh and cosh
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> tanh(utils::number<T, n, f, op, up> const& val)
    {
        typedef utils::number<T, n, f, op, up> type;

        return type(std::sinh(val) / std::cosh(val));
    }
}
