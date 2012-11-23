/// @brief provides CORDIC for arccos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

#include <boost/type_traits/is_floating_point.hpp>

#include <boost/integer.hpp>

namespace core {
    template<typename T>
    class atan_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class atan_of<fixed_point<T, n, f, op, up> >
    {
    public:
        typedef typename asin_of<fixed_point<T, n, f, op, up> >::type type;
    }
}

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::atan_of<core::fixed_point<T, n, f, op, up> >::type atan(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        typedef fp::atan_type result_type;
        typedef core::cordic::lut<f, fp> lut;

        static lut const angles = lut::circular();

        // vectoring mode: see page 10, table 24.2
        // shift sequence is just 0, 1, ... (circular coordinate system)
        result_type x(1.0), y(val), z(0.0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            int const sign = ((x.value() > 0)? +1 : -1) * ((y.value() > 0)? +1 : -1);

            result_type::word_type const store(x.value());
            x = x + result_type::wrap(sign * (y.value() >> i));
            y = y - result_type::wrap(sign * (store >> i));
            z = (sign > 0)? z + angles[i] : z - angles[i];
        }

        return z;
    }
}
