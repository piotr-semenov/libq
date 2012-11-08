/// @brief provides std::floor stuff in case of fixed-point numbers

#include <boost/integer/integer_mask.hpp>

namespace std {
    /// @brief floor
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> floor(core::fixed_point<T, n, f, op, up> const& x)
    {
        typedef core::fixed_point<T, n, f, op, up> type;
        static T const mask(boost::high_bits_mask_t<std::numeric_limits<T>::digits -
            f>::least);

        return type::wrap(x.value() & mask);
    }
}
