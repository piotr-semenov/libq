/// @brief provides CORDIC for cosh function

#include <boost/type_traits/is_floating_point.hpp>

#include <boost/integer.hpp>

namespace core {
    template<typename T>
    class cosh_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class cosh_of<fixed_point<T, n, f, op, up> >
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
    /// @brief computes cosh as sum of exponents
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::cosh_of<core::fixed_point<T, n, f, op, up> >::type cosh(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp_type;
        typedef core::cosh_of<fp_type>::type result_type;

        result_type x = std::exp(val) + std::exp(-val);
        as_native(x) >>= 1u;

        return x;
    }
}
