/// @brief provides CORDIC for arcsinh function

#include <boost/type_traits/is_floating_point.hpp>

#include <boost/integer.hpp>

namespace libq {
    template<typename T>
    class asinh_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class asinh_of<fixed_point<T, n, f, op, up> >
    {
    public:
        typedef typename fixed_point<T, n, f, op, up>::log_type type;
    };
}

namespace std {

    /// @brief computes arcinh as logarithm
    template<typename T, size_t n, size_t f, class op, class up>
    typename libq::asinh_of<libq::fixed_point<T, n, f, op, up> >::type asinh(libq::fixed_point<T, n, f, op, up> val)
    {
        typedef libq::fixed_point<T, n, f, op, up> fp_type;
        typedef libq::asinh_of<fp_type>::type result_type;

        return result_type(
            std::log(
                libq::s(
                    std::sqrt(val * val + 1u)
                ) + val
            )
        );
    }
}
