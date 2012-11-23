/// @provides in    ferred types for elementary functions

#include <boost/integer/static_min_max.hpp>
#include <boost/integer/static_log2.hpp>

namespace core {
    namespace {
        using boost::mpl::eval_if;
        using boost::mpl::and_;
        using boost::static_unsigned_max;

        using boost::static_log2;
    }

/// LOG
    template<typename T, size_t n, size_t f, class op, class up>
    class fixed_point;

    template<typename T>
    class get_
    {
    public:
        typedef double log_type;
        typedef double sqrt_type;
        typedef double max_signed_type;

        typedef double sin_type;
        typedef sin_type cos_type;
        typedef double tan_type;

        typedef double arc_type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class get_<fixed_point<T, n, f, op, up> >
    {
        typedef fixed_point<T, n, f, op, up> operand_type;

        struct sqrt_info
        {
            static size_t const integer_bits =
                ((n - f) & 1u)? ((n - f)/2u) + 1u : (n - f)/2u;
            static size_t const fractional_bits =
                (f & 1u)? (f/2u) + 1u : (f/2u);
            static size_t const total_bits = integer_bits + fractional_bits;
        };

        struct log_info
        {
            static size_t const extra_bits =
                static_unsigned_max<static_log2<f>::value + 1u, n + static_log2<n>::value + 1u>::value;

            struct can_expand
            {
                enum { value = (f != 0) &&
                    (n + log_info::extra_bits + 1u < std::numeric_limits<boost::intmax_t>::digits) };
            };

            struct expanded
            {
                typedef fixed_point<
                    typename boost::int_t<n + extra_bits + 1u>::least,
                    n + extra_bits,
                    f,
                    op,
                    up
                > type;
            };

            struct same
            {
                typedef operand_type type;
            };
        };

        struct sin_info
        {
            struct can_expand
            {
                enum { value = (f + 1u + 1u < std::numeric_limits<boost::intmax_t>::digits) };
            };

            struct expanded
            {
                typedef fixed_point<
                    typename boost::int_t<f + 1u + 1u>::least,
                    f + 1u,
                    f,
                    op,
                    up
                > type;
            };

            struct reduced
            {
                typedef fixed_point<
                    typename boost::int_t<f + 1u>::least,
                    f,
                    f - 1u,
                    op,
                    up
                > type;
            };
        };

        struct asin_info
        {
            static size_t const available_bits = std::numeric_limits<boost::intmax_t>::digits - f;

            struct can_expand
            {
                enum { value = (available_bits > 2u) };
            };

            struct expanded
            {
                typedef fixed_point<
                    typename boost::int_t<f + 2u + 1u>::least,
                    f + 2u,
                    f,
                    op,
                    up
                > type;
            };

            struct reduced
            {
                typedef fixed_point<
                    typename boost::int_t<f + 2u>::least,
                    f + available_bits,
                    f + available_bits - 2u,
                    op,
                    up
                > type;
            };
        };

        struct acos_info
        {
            static size_t const available_bits = std::numeric_limits<boost::intmax_t>::digits - f;

            struct can_expand
            {
                enum { value = (available_bits > 3u) };
            };

            struct expanded
            {
                typedef fixed_point<
                    typename boost::int_t<f + 3u + 1u>::least,
                    f + 3u,
                    f,
                    op,
                    up
                > type;
            };

            struct reduced
            {
                typedef fixed_point<
                    typename boost::int_t<f + 3u>::least,
                    f + available_bits,
                    f + available_bits - 3u,
                    op,
                    up
                > type;
            };
        };

    public:
        typedef typename eval_if<
            typename log_info::can_expand,
            typename log_info::expanded,
            typename log_info::same
        >::type log_type;

        // exists always
        typedef fixed_point<
            typename boost::uint_t<sqrt_info::total_bits>::least,
            sqrt_info::total_bits,
            sqrt_info::fractional_bits,
            op,
            up
        > sqrt_type;

        typedef fixed_point<boost::uintmax_t,
            std::numeric_limits<boost::uintmax_t>::digits, f, op, up> max_unsigned_type;

        typedef fixed_point<boost::intmax_t,
            std::numeric_limits<boost::intmax_t>::digits, f, op, up> max_signed_type;

        typedef typename eval_if<
            typename sin_info::can_expand,
            typename sin_info::expanded,
            typename sin_info::reduced
        >::type sin_type;
        typedef sin_type cos_type;
        typedef typename quotient<sin_type, cos_type>::type tan_type;

        typedef max_unsigned_type exp_type;

        typedef typename eval_if<
            typename asin_info::can_expand,
            typename asin_info::expanded,
            typename asin_info::reduced
        >::type asin_type;
        typedef asin_type atan_type;

        typedef typename eval_if<
            typename acos_info::can_expand,
            typename acos_info::expanded,
            typename acos_info::reduced
        >::type acos_type;

        typedef typename get_<operand_type>::log_type asinh_type;
    };
}
