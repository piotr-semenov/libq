/// @brief provides stuff for std::tan in case of fixed-point numbers

#include <boost/type_traits/is_floating_point.hpp>

#include <boost/integer.hpp>

namespace core {
    template<typename T>
    class tan_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class tan_of<fixed_point<T, n, f, op, up> >
    {
        typedef fixed_point<T, n, f, op, up> fp_type;

    public:
        typedef typename quotient_of<
            typename sin_of<fp_type>::type,
            typename cos_of<fp_type>::type
        >::type type;
    };
}

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::tan_of<core::fixed_point<T, n, f, op, up> >::type tan(core::fixed_point<T, n, f, op, up> val)
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
