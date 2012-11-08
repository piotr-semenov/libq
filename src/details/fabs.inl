/// @brief provides std::fabs stuff in case of fixed-point number

namespace std {
    /// @brief computes absolute value in case of fixed-point numbers
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> fabs(core::fixed_point<T, n, f, op, up> const& x)
    {
        typedef core::fixed_point<T, n, f, op, up> type;

        return (x < type(0)) ? -x : x;
    }
}
