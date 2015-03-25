// lut.hpp
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file lut.hpp

 Provides a look-up table (LUT) for CORDIC stuff

 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_LIBQ_CORDIC_LUT_HPP_
#define INC_LIBQ_CORDIC_LUT_HPP_

#include <array>

namespace libq {
namespace cordic {

/*!
 \brief look-up table for CORDIC algorithms
 \tparam n
 \tparam Q
*/
template<std::size_t n, typename Q>
class lut
    :    public std::array<Q, n>
{
    typedef std::array<Q, n> base_class;
    typedef lut<n, Q> this_class;

    lut(base_class const& _dat)
        : base_class(_dat){}

public:
    typedef Q fixed_point_type; ///< type of fixed-point numbers

    enum: std::size_t {
        dim = n ///< size of LUT
    };

    /*!
     \brief creates the LUT for angles in case of CORDIC rotations are performed
     for circular coordinates
    */
    static this_class circular();

    /*!
     \brief creates the LUT for angles in case of CORDIC rotations are performed
     in hyperbolic coordinates.
     \note this does not use the repeated iterations. All values are unique
    */
    static this_class hyperbolic_wo_repeated_iterations();

    /*!
     \brief creates the LUT of \f$2^{2^{-i}}\f$ for n positions
     \note this LUT is used for exp function
    */
    static this_class pow2();

    /*!
     \brief creates the LUT of \f$\frac1{2^\frac1{2^i}}\f$ for n positions
     \note this LUT is used for log2 function
    */
    static this_class inv_pow2();

    /*!
     \brief computes the scale of n CORDIC-rotations in circular coordinates
    */
    static double circular_scale(std::size_t const _n);

    /*!
     \brief creates the LUT for scales of n CORDIC-rotations in case of circular
     coordinates
    */
    static this_class circular_scales();

    /*!
     \brief computes the scale of n CORDIC-rotations in hyperbolic coordinates.
     \note this uses repeated iterations for convergence
    */
    static double hyperbolic_scale_with_repeated_iterations(std::size_t _n);
};
} // cordic
} // libq

#include "arctan_lut.inl"
#include "arctanh_lut.inl"

#include "pow2_lut.inl"
#include "inv_pow2_lut.inl"

#include "circular_scales.inl"
#include "hyperbolic_scale.inl"

#endif // INC_LIBQ_CORDIC_LUT_HPP_
