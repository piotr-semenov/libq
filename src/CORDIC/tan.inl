/// @brief provides stuff for std::tan in case of fixed-point numbers

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::fixed_point<T, n, f, op, up>::tan_type tan(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;

        fp::sin_type const sin = std::sin(val);
        fp::cos_type const cos = std::cos(val);

        if (cos == fp::cos_type(0)) {
            throw std::exception("tan: cos is zero");
        }

        return fp::tan_type(sin / cos);
    }
}
