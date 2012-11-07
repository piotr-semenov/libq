/// @brief provides std::fabs stuff in case of fixed-point number

namespace std {
    /// @brief computes absolute value
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> fabs(utils::number<T, n, f, op, up> const& x)
    {
        typedef utils::number<T, n, f, op, up> type;

        return (x < type(0)) ? -x : x;
    }
}
