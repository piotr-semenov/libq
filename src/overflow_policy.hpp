/// @brief provides 

#ifndef INC_OVERFLOW_POLICY_HPP_
#define iNC_OVERFLOW_POLICY_HPP_

#include <boost/cstdint.hpp>

#include <limits>
#include <stdexcept>

namespace utils {
    /// @brief overflow policy for fixed-point arithmetics
    template<typename fixed_point>
    class overflow_policy
    {
        typedef overflow_policy<fixed_point> this_class;

    public:
        typedef typename fixed_point::value_type value_type;
        typedef typename fixed_point::bounds bounds;

        /// @brief policy to throw an exception in case the overflow has occured
        class exception
        {
        public:
            /// @brief checks if any integral value belongs to range of current
            /// fixed-point format. If it is not it will throw overflow exceptions.
            /// @throw std::overflow_error
            template<typename T>
            static inline value_type overflow_check(T val)
            {
                if (val > T(0) && boost::uintmax_t(val) > bounds::max) {
                    throw std::overflow_error();
                }

                return static_cast<value_type>(val);
            }
        };

        /// @brief policy to do nothing in case the overflow has occured
        class just_do
        {
        public:
            /// @brief converts integer to fixed point value type. If it is does not
            /// fit fixed point value type range than it performs by modulus computing.
            template<typename T>
            static inline value_type overflow_check(T val)
            {
                return static_cast<value_type>(val % bounds::max);
            }
        };
    };
}

#endif
