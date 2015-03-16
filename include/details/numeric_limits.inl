/// @brief provides stuff for std::numeric_limits

namespace std {
#define M_LOG102 0.301029995663981195214

    /// @brief numeric limits specialization for fixed-point
    template<typename T, size_t n, size_t f, class op, class up>
    class numeric_limits<libq::fixed_point<T, n, f, op, up> >
    {
        typedef libq::fixed_point<T, n, f, op, up> type;

        template<class U>
        struct has_exceptions
        {
            enum { value = false };
        };

        template<>
        struct has_exceptions<libq::do_exception>
        {
            enum { value = true };
        };

    public:
        /// @brief type does not support subnormal values
        static float_denorm_style const has_denorm = denorm_absent;
        static bool const has_denorm_loss = false;

        static bool const has_infinity = false;
        static bool const has_quiet_NaN = false;
        static bool const has_signaling_NaN = false;

        /// @brief type represents finite sets of values
        static bool const is_bounded = true;

        /// @brief type uses an exact representation of values
        static bool const is_exact = true;

        static const bool is_iec599 = false;
        static const bool is_integer = false;

        /// @brief note: type handles overflows with modulo arithmetic
        static bool const is_modulo = !has_exceptions<op>::value;

        static bool const is_signed = std::numeric_limits<typename type::word_type>::is_signed;

        /// @brief this is the explicit specialization of std::numeric_limits for 
        ///        fixed-point number type
        static bool const is_specialized = true;

        /// @brief checks if underflow occurring before rounding
        static bool const tinyless_before = has_exceptions<up>::value;

        /// @brief checks if type has exceptions for arithmetics troubles
        static bool const traps = has_exceptions<up>::value;

        /// @brief type rounds the values by rejecting bits
        static float_round_style const round_style = round_to_nearest;

        static int const digits = n;
        static int const digits10 = static_cast<int>(n * M_LOG102 + 0.5);

        static int const max_exponent = n-f;
        static int const max_exponent10 = static_cast<int>(max_exponent * M_LOG102);
        static int const min_exponent = f;
        static int const min_exponent10 = static_cast<int>(min_exponent * M_LOG102);
        static int const radix = 2;

        /// @brief minimum value that can be achieved by fixed-point type
        static type min()
        {
            typedef typename type::word_type word_type;

            return type::wrap(static_cast<word_type>(type::bounds::min));
        }

        /// @brief maximum value that can be achieved by fixed-point type
        static type max()
        {
            typedef typename type::word_type word_type;

            return type::wrap(static_cast<word_type>(type::bounds::max));
        }

        /// @brief returns the machine epsilon, that is, the difference between
        /// 1.0 and the next value representable by the fixed-point type
        static type epsilon(){ return type::wrap(1u); }

        /// @brief the maximum rounding error for fixed-point type
        static type round_error(){ return type(0.5); }

        static type denorm_min(){ return min(); }
        static type infinity(){ return type(max()); }
        static type quiet_NaN(){ return type(0); }
        static type signaling_NaN(){ return type(0); }
    };

#undef M_LOG10_2
}
