/// @brief provides stuff for std::tan in case of fixed-point numbers

#include <exception>

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> tan(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> type;

        type const sin = std::sin(val);
        type const cos = std::cos(val);

        if (cos == type(0)) {
            throw std::exception("tan: cos is zero");
        }

        return type(sin / cos);
    }
}
