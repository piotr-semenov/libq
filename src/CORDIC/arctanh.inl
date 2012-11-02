/// @brief provides CORDIC for arcsinh function

namespace std {
    /// @brief computes arccosh as logarithm
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> atanh(utils::number<T, n, f, op, up> const& val)
    {
        typedef utils::number<T, n, f, op, up> type;
        assert(("arctanh: argument has to be from range [-1.0, 1.0]",
            std::fabs(val) <= 1.0));

        type x = type(std::log(val + 1) - std::log(type(1) - val));
        as_native(x) >>= 1u;
        x = type(x / type::CONST_LOG2E);

        return x;
    }
}
