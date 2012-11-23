/// @brief provides CORDIC for arctanh function

#include <boost/type_traits/is_floating_point.hpp>

#include <boost/integer.hpp>

namespace core {
    template<typename T>
    class atanh_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class atanh_of<fixed_point<T, n, f, op, up> >
    {
    public:
        typedef typename diff<typename fixed_point<T, n, f, op, up>::log_type>::type type;
    };
}

namespace std {
    /// @brief computes arctanh as logarithms
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::atanh_of<core::fixed_point<T, n, f, op, up> >::type atanh(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp_type;
        typedef core::atanh_of<fp_type>::type result_type;

        assert(("arctanh: argument has to be from range [-1.0, 1.0]", std::fabs(val) <= 1.0));

        result_type x(std::log(val + 1u) - std::log(fp_type(1) - val));
        core::as_native(x) >> 1u;

        return x;
    }
}
