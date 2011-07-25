#ifndef INC_STATIC_POW_HPP_
#define INC_STATIC_POW_HPP_

#include <boost/integer.hpp>

namespace utils {
    template<size_t n, size_t p>
    struct static_pow
    {
        static boost::uintmax_t const value = n * static_pow<n, p-1>::value;
    };

    template<size_t n>
    struct static_pow<n, 1>
    {
        static boost::uintmax_t const value = n;
    };
}

#endif
