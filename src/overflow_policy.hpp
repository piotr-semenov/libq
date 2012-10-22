/// @brief provides 

#ifndef INC_OVERFLOW_POLICY_HPP_
#define iNC_OVERFLOW_POLICY_HPP_

#include <boost/cstdint.hpp>

#include <limits>
#include <string>
#include <stdexcept>

namespace utils {
    /// @brief overflow policy for fixed-point arithmetics
    class overflow_policy
    {
    public:
        /// @brief policy to throw an exception in case the overflow has occured
        class exception
        {
        public:
            /// @brief checks if any integral value belongs to range of current
            /// fixed-point format. If it is not it will throw overflow exceptions.
            /// @throw std::overflow_error
            template<typename fixed_point, typename T>
            static inline typename fixed_point::value_type checked_convert(T val)
            {
                static std::string const m0("positive value is grater than upper bound of range");
                static std::string const m1("negative value is less than lower bound of range");

                if (val > T(0) && boost::uintmax_t(val) > fixed_point::bounds::max) {
                    throw std::overflow_error(m0);
                }

                if (fixed_point::is_signed && boost::intmax_t(val) < fixed_point::bounds::min) {
                    throw std::overflow_error(m1);
                }

                return static_cast<typename fixed_point::value_type>(val);
            }
        };

        /// @brief policy to do nothing in case the overflow has occured
        class just_do
        {
        public:
            /// @brief converts integer to fixed point value type. If it is does not
            /// fit fixed point value type range than it performs by modulus computing.
            template<typename fixed_point, typename T>
            static inline typename fixed_point::value_type checked_convert(T val)
            {
                typedef typename fixed_point::value_type word_type;
                boost::uintmax_t const max = fixed_point::bounds::max;

                word_type u(0);

                if (fixed_point::is_signed) {
                    if (val > boost::intmax_t(max)) {
                        u = static_cast<word_type>((val & max) - max);
                    }
                    else {
                        int const sign_bit(val > 0 ? 1 : -1);
                        u = sign_bit * static_cast<word_type>((sign_bit * val) & max);
                    }
                }
                else {
                    u = static_cast<word_type>(val & max);
                }

                return u;
            }
        };
    };
}

#endif
