// lut.hpp
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file lut.hpp

 Provides a look-up table (LUT) for CORDIC stuff.

 \ref See H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures".
*/

#ifndef INC_LIBQ_CORDIC_LUT_HPP_
#define INC_LIBQ_CORDIC_LUT_HPP_

#include <array>

namespace libq {
namespace cordic {

/*!
 \brief Look-up table for CORDIC algorithms.
 \tparam n
 \tparam Q
*/
template<std::size_t n, typename Q>
class lut
    : public std::array<Q, n> {
    using base_class = std::array<Q, n>;
    using this_class = lut<n, Q>;

    explicit lut(base_class const& _dat)
        : base_class(_dat) {
    }

 public:
    using fixed_point_type = Q;  ///< type of fixed-point numbers

    enum: std::size_t {
        dim = n  ///< size of LUT
    };


    /*!
     \brief Creates the LUT for angles in case of CORDIC rotations are
     performed for circular coordinates.
    */
    static this_class circular();


    /*!
     \brief Creates the LUT for angles in case of CORDIC rotations are performed
     in hyperbolic coordinates.
     \note This does not use the repeated iterations. All values are unique.
    */
    static this_class hyperbolic_wo_repeated_iterations();


    /*!
     \brief Creates the LUT of \f$2^{2^{-i}}\f$ for n positions.
     \note This LUT is used for exp function.
    */
    static this_class pow2();


    /*!
     \brief Creates the LUT of \f$\frac1{2^\frac1{2^i}}\f$ for n positions.
     \note This LUT is used for log2 function.
    */
    static this_class inv_pow2();


    /*!
     \brief Computes the scale of n CORDIC-rotations in circular coordinates.
    */
    static double circular_scale(std::size_t const _n);


    /*!
     \brief Creates the LUT for scales of n CORDIC-rotations in case of circular
     coordinates.
    */
    static this_class circular_scales();


    /*!
     \brief Computes the scale of n CORDIC-rotations in hyperbolic coordinates.
     \note This uses repeated iterations for convergence.
    */
    static double hyperbolic_scale_with_repeated_iterations(std::size_t _n);
};
}  // namespace cordic
}  // namespace libq

#include "arctan_lut.inl"
#include "arctanh_lut.inl"

#include "pow2_lut.inl"
#include "inv_pow2_lut.inl"

#include "circular_scales.inl"
#include "hyperbolic_scale.inl"

#endif  // INC_LIBQ_CORDIC_LUT_HPP_
