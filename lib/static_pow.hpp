#ifndef INC_STATIC_POW_HPP_
#define INC_STATIC_POW_HPP_

#include <boost/integer.hpp>
#include <boost/static_assert.hpp>

namespace utils {
    template<size_t n, size_t p>
    struct static_pow
    {
        BOOST_STATIC_ASSERT((p >= 0));
        static boost::uintmax_t const value = n * static_pow<n, p-1>::value;
    };

    template<size_t n>
    struct static_pow<n, 0>
    {
        static boost::uintmax_t const value = 1;
    };
}

#endif
