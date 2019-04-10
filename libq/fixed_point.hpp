// fixed_point.hpp
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/**
 @file fixed_point.hpp

 @brief Provides the fixed-point numbers of any format and CORDIC-based
 overloading of STD math as well.
*/

#ifndef INC_LIBQ_FIXED_POINT_HPP_
#define INC_LIBQ_FIXED_POINT_HPP_

#include "libq/loop_unroller.hpp"

#include "libq/details/sum_traits.hpp"
#include "libq/details/mult_of.hpp"
#include "libq/details/div_of.hpp"
#include "libq/details/sign.hpp"
#include "libq/details/ceil.hpp"
#include "libq/details/fabs.hpp"
#include "libq/details/floor.hpp"
#include "libq/details/round.hpp"
#include "libq/details/remainder.hpp"
#include "libq/details/fmod.hpp"
#include "libq/details/numeric_limits.hpp"
#include "libq/details/type_traits.hpp"

#include "libq/CORDIC/lut/lut.hpp"
#include "libq/CORDIC/log.hpp"
#include "libq/CORDIC/sqrt.hpp"
#include "libq/CORDIC/sin.hpp"
#include "libq/CORDIC/cos.hpp"
#include "libq/CORDIC/tan.hpp"
#include "libq/CORDIC/exp.hpp"
#include "libq/CORDIC/sinh.hpp"
#include "libq/CORDIC/cosh.hpp"
#include "libq/CORDIC/tanh.hpp"
#include "libq/CORDIC/acos.hpp"
#include "libq/CORDIC/asin.hpp"
#include "libq/CORDIC/atan.hpp"
#include "libq/CORDIC/asinh.hpp"
#include "libq/CORDIC/acosh.hpp"
#include "libq/CORDIC/atanh.hpp"

#endif  // INC_LIBQ_FIXED_POINT_HPP_
