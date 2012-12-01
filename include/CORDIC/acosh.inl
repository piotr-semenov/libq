/// @brief provides CORDIC for arcsinh function

#include <boost/type_traits/is_floating_point.hpp>

#include <boost/integer.hpp>

namespace core {
    template<typename T>
    class acosh_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class acosh_of<fixed_point<T, n, f, op, up> >
    {
    public:
        typedef typename fixed_point<T, n, f, op, up>::log_type::to_unsigned_type type;
    };
}

namespace std {
    /// @brief computes arccosh as logarithm
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::acosh_of<core::fixed_point<T, n, f, op, up> >::type acosh(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp_type;
        typedef core::acosh_of<fp_type>::type result_type;

        assert(("acosh: illegal argument", val > fp_type(1.0)));

        return result_type(
            std::log(
                val + std::sqrt(val * val - 1)
            )
        );
    }
}
