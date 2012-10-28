#include <exception>

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> tan(utils::number<T, n, f, op, up> const& val)
    {
        typedef utils::number<T, n, f, op, up> type;

        type const sin = std::sin(val);
        type const cos = std::cos(val);

        if (cos == type(0)) {
            throw std::exception("tan: cos is zero");
        }

        return type(sin / cos);
    }
}
