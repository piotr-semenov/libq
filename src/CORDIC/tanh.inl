/// @brief provides CORDIC for tanh function

#include <boost/type_traits/is_floating_point.hpp>

#include <boost/integer.hpp>

namespace core {
    template<typename T>
    class tanh_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class tanh_of<fixed_point<T, n, f, op, up> >
    {
        typedef fixed_point<T, n, f, op, up> fp_type;

    public:
        typedef typename fixed_point<
            typename boost::int_t<1u + f>::least,
            1u + f,
            f,
            op,
            up
        > type;
    };
}

namespace std {
    /// @brief computes tanh as ratio of sinh and cosh
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::atanh_of<core::fixed_point<T, n, f, op, up> >::type tanh(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp_type;

        fp_type::sinh_type const a(std::sinh(val));
        fp_type::cosh_type const b(std::cosh(val));

        return core::tanh_of<fp_type>::type(std::sinh(val) / std::cosh(val));
    }
}
