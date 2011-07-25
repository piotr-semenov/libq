/// @provides (Co)ordinate (R)otation (Di)gital (C)omputer for elementary functions fixed-point implementation

#ifndef INC_CORDIC_HPP_
#define INC_CORDIC_HPP_

#include <complex>
#include <boost/array.hpp>

namespace utils { namespace cordic {
    template<size_t n>
    class cordic
    {
        typedef cordic<n> this_class;
        typedef boost::array<double, n+1> lut_type;

        /// @brief builds look-up table of angles {x_i} that
        ///        tan(x_i) = 2^{-i}
        static lut_type build();

    public:
        static size_t const stages = n;

        cordic()
            :    m_arctan_table(build()){};

        /// @brief performs multiple rotation in arithmetics that type T provides
        template<typename T>
        T magnitude(T const x) const;

        /// @brief 
        template<typename T>
        std::complex<T> rotate(std::complex<T> x, T angle) const;

    private:
        lut_type m_arctan_table;
    };
}}

#include "./../../Common/cordic/cordic.inl"

#endif
