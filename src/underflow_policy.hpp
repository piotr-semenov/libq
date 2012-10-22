/// @brief provides 

#ifndef INC_UNDERFLOW_POLICY_HPP_
#define iNC_UNDERFLOW_POLICY_HPP_

#include <boost/cstdint.hpp>

#include <limits>

#include <string>
#include <stdexcept>

namespace utils {
    /// @brief underflow policy for fixed-point arithmetics
    class underflow_policy
    {
    public:
        /// @brief policy to throw an exception in case the underflow has occured
        class exception
        {
        public:
            /// @brief checks if any floating-point/another fixed-point value 
            /// can be represented in current fixed-point format. If it is not it
            /// will throw underflow exceptions.
            /// @throw boost::numeric::positive_overflow
            /// @throw boost::numeric::negative_overflow
            template<typename fixed_point, typename T>
            static inline typename fixed_point::value_type checked_convert(T val)
            {
                static std::string const m("the value is too small to be captured with fixed-point type");

                if (val != T(0) && as_native(fixed_point(val)) == 0) {
                    throw std::underflow_error(m);
                }

                return static_cast<typename fixed_point::value_type>(val);
            }
        };

        /// @brief policy to do nothing in case the underflow has occured
        class just_do
        {
        public:
            /// @brief converts integer to fixed point value type. If it is does not
            /// fit fixed point value type range than it performs by modulus computing.
            template<typename fixed_point, typename T>
            static inline typename fixed_point::value_type checked_convert(T val)
            {
                return static_cast<typename fixed_point::value_type>(val %
                    fixed_point::bounds::max);
            }
        };
    };
}

#endif
