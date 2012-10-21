/// @brief provides 

#ifndef INC_UNDERFLOW_POLICY_HPP_
#define iNC_UNDERFLOW_POLICY_HPP_

#include <boost/cstdint.hpp>

#include <limits>

#include <string>
#include <stdexcept>

namespace utils {
    /// @brief underflow policy for fixed-point arithmetics
    template<typename fixed_point_type>
    class underflow_policy
    {
        typedef underflow_policy<fixed_point_type> this_class;

    public:
        typedef typename fixed_point_type::value_type value_type;

        /// @brief policy to throw an exception in case the underflow has occured
        class exception
        {
        public:
            /// @brief checks if any floating-point/another fixed-point value 
            /// can be represented in current fixed-point format. If it is not it
            /// will throw underflow exceptions.
            /// @throw boost::numeric::positive_overflow
            /// @throw boost::numeric::negative_overflow
            template<typename T>
            static inline value_type underflow_check(T val)
            {
                static std::string const m0("negative value is less than lower bound of range");
                static std::string const m1("the value is too small to be captured with fixed-point type");

                if (fixed_point_type::is_signed && boost::intmax_t(val) < bounds::min) {
                    throw std::underflow_error(m0);
                }

                if (val != T(0) && as_native(fixed_point_type(val)) == 0) {
                    throw std::underflow_error(m1);
                }

                return static_cast<value_type>(val);
            }
        };

        /// @brief policy to do nothing in case the underflow has occured
        class just_do
        {
        public:
            /// @brief converts integer to fixed point value type. If it is does not
            /// fit fixed point value type range than it performs by modulus computing.
            template<typename T>
            static inline value_type underflow_check(T val)
            {
                return static_cast<value_type>(val % bounds::max);
            }
        };
    };
}

#endif
