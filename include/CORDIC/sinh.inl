/// @brief provides CORDIC for sinh function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures" and 
/// J.S. Walther, "A Unified Algorithm for Elementary Functions"

#include <boost/type_traits/is_floating_point.hpp>

namespace core {
    template<typename T>
    class sinh_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class sinh_of<fixed_point<T, n, f, op, up> >
    {
    public:
        typedef fixed_point<
            boost::intmax_t,
            std::numeric_limits<boost::intmax_t>::digits,
            f,
            op,
            up
        > type;
    };
}

namespace std {
    /// @brief computes sinh as sum of exponents
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::sinh_of<core::fixed_point<T, n, f, op, up> >::type sinh(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp_type;
        typedef core::sinh_of<fp_type>::type result_type;

        result_type x = result_type(std::exp(val) - std::exp(-val));
        core::as_native(x) >>= 1u;

        return x;
    }
}
