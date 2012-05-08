/// @provides look-up table for CORDIC for hyperbolic functions

#ifndef INC_ARCTAN_LUT_HPP_
#define INC_ARCTAN_LUT_HPP_

#include <boost/array.hpp>

namespace utils { namespace cordic {
    template<size_t n>
    class arctanh_lut
        :    public boost::array<double, n>
    {
        typedef boost::array<double, n> base_class;
        typedef arctanh_lut<n> this_class;

        static base_class build();

    public:
        static size_t const dim = n;
        typedef base_class lut_type;

        arctanh_lut()
            :    this_class(build()){};
    };
}}

#include "./../../Common/cordic/arctanh_lut.inl"

#endif
