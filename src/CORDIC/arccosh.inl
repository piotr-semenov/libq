/// @brief provides CORDIC for arcsinh function

namespace std {
    /// @brief computes arccosh as logarithm
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> acosh(utils::number<T, n, f, op, up> const& val)
    {
        typedef utils::number<T, n, f, op, up> type;
        assert(("acosh: illegal argument", val > type(1.0)));

        type const arg(val + type(std::sqrt(val * val - 1)));
        type x = type(std::log(arg) / type::CONST_LOG2E);

        return x;
    }
}
